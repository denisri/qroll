/***************************************************************************
                          dsp.cc
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2010 by Denis Rivière
    email                : nudz@free.fr
                           http://nudz.free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifdef SOMA_SOUND_ALSA

#include "somasoundalsa.h"
#include "soundbank.h"
#include "soundslot.h"
#include "sounditerator.h"
#include "diffcode.h"
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace soma;
using namespace audiq;
using namespace std;


const unsigned          SomaSoundAlsa::MaxSameSample = 3;


struct SomaSoundAlsa::Private
{
  Private();

  snd_pcm_t *handle;
  unsigned sampleSize;
  unsigned nchannels;
  bool signedSamples;
  unsigned bufferTime;
};


SomaSoundAlsa::Private::Private()
  : handle( 0 ), sampleSize( 1 ), nchannels( 1 ), signedSamples( false ),
    bufferTime( 25000 )
{
}


SomaSoundAlsa::SndReq::~SndReq()
{
  delete iterator;
}


SomaSoundAlsa::SomaSoundAlsa() : ok( true ), bufferSize( 0 ), buffer( 0 ),
  updateThrd( 0 ), threadRunning( false ), freqDsp( 22050 ), d( new Private )
{
  init();
}


SomaSoundAlsa::~SomaSoundAlsa()
{
  close();
  delete[] buffer;
  delete d;
}


void SomaSoundAlsa::init()
{
  ok = true;
  RRSoundProcessor::init();

#define QR_SOUNDDEV     "default"

  cout << "Sound device : " << QR_SOUNDDEV << endl;

  // Open the soundcard device.
  snd_pcm_t *handle;
  int err;

  if( ( err = snd_pcm_open( &handle, QR_SOUNDDEV, SND_PCM_STREAM_PLAYBACK, 0 ) )
    < 0 )
  {
    cerr << "ALSA sound open error: " << snd_strerror(err) << endl;
    ok = false;
    return;
  }

  snd_pcm_hw_params_t *hw_params;
  if( ( err = snd_pcm_hw_params_malloc( &hw_params ) ) < 0 )
  {
    cerr << snd_strerror(err) << endl;
    snd_pcm_close(handle);
    ok = false;
    return;
  }

  // mono, 8 bits, 22 kHz
  snd_pcm_format_t format = SND_PCM_FORMAT_U8;
  if( d->sampleSize == 2 )
  {
    if( d->signedSamples )
      format = SND_PCM_FORMAT_S16;
    else
      format = SND_PCM_FORMAT_U16;
  }
  else if( d->signedSamples )
    format = SND_PCM_FORMAT_S8;
  if( ( err = snd_pcm_hw_params_any( handle, hw_params ) ) < 0 )
  {
    cerr << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  if( ( err = snd_pcm_hw_params_set_access( handle, hw_params,
    SND_PCM_ACCESS_RW_INTERLEAVED ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  if( ( err = snd_pcm_hw_params_set_format( handle, hw_params,
    format ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  if( ( err = snd_pcm_hw_params_set_rate_near( handle, hw_params, &freqDsp,
    0 ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  if( ( err = snd_pcm_hw_params_set_channels( handle, hw_params,
    d->nchannels ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  int dir = 1;

  err = snd_pcm_hw_params_set_buffer_time_near( handle, hw_params,
      &d->bufferTime, &dir);
  if (err < 0)
  {
    /*
    cerr << "Unable to set buffer time " << bufferTime << " for playback: "
      << snd_strerror(err) << endl;
    */
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  // cout << "buffer time: " << d->bufferTime << endl;

  if( ( err = snd_pcm_hw_params( handle, hw_params ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(handle);
    ok = false;
    return;
  }

  snd_pcm_uframes_t bufsz = 0;
  err = snd_pcm_hw_params_get_buffer_size( hw_params, &bufsz );
  if (err < 0)
  {
    cerr << "Unable to get buffer size for playback: "
      << snd_strerror(err) << endl;
    // why doesn't this work ???
    bufsz = ((long long) d->bufferTime) * freqDsp / 1000000;
  }
  // cout << "obtained buffer size : " << bufsz << endl;

  snd_pcm_hw_params_free( hw_params );

  if( ( err = snd_pcm_prepare( handle ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_close(handle);
    ok = false;
    return;
  }

  bufferSize = bufsz;
  cout << "buffer size: " << bufferSize << endl;
  d->handle = handle;

  delete[] buffer;
  buffer = new unsigned char[ bufferSize ];
  listLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

  loadSounds();
}


void SomaSoundAlsa::setBufferTime( float ms )
{
  d->bufferTime = (unsigned) ( ms * 1000 );
  if( !d->handle )
    return;

  int err;
  snd_pcm_hw_params_t *hw_params;
  if( ( err = snd_pcm_hw_params_malloc( &hw_params ) ) < 0 )
  {
    cerr << snd_strerror(err) << endl;
    snd_pcm_close(d->handle);
    d->handle = 0;
    return;
  }

  int dir = 1;

  err = snd_pcm_hw_params_set_buffer_time_near( d->handle, hw_params,
      &d->bufferTime, &dir);
  if (err < 0)
  {
    /*
    cerr << "Unable to set buffer time " << d->bufferTime << " for playback: "
      << snd_strerror(err) << endl;
    */
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(d->handle);
    d->handle = 0;
    return;
  }
  // cout << "buffer time: " << bufferTime << endl;

  if( ( err = snd_pcm_hw_params( d->handle, hw_params ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_hw_params_free( hw_params );
    snd_pcm_close(d->handle);
    d->handle = 0;
    return;
  }

  snd_pcm_uframes_t bufsz = 0;
  err = snd_pcm_hw_params_get_buffer_size( hw_params, &bufsz );
  if (err < 0)
  {
    cerr << "Unable to get buffer size for playback: "
      << snd_strerror(err) << endl;
    // why doesn't this work ???
    bufsz = ((long long) d->bufferTime) * freqDsp / 1000000;
  }
  // cout << "obtained buffer size : " << bufsz << endl;

  snd_pcm_hw_params_free( hw_params );

  if( ( err = snd_pcm_prepare( d->handle ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    snd_pcm_close(d->handle);
    d->handle = 0;
    return;
  }

  bufferSize = bufsz;
  cout << "buffer size: " << bufferSize << endl;

  delete[] buffer;
  buffer = new unsigned char[ bufferSize ];
}


void SomaSoundAlsa::setFrequency( int freq )
{
  freqDsp = freq;
}


void SomaSoundAlsa::setChannels( int ch )
{
  d->nchannels = ch;
}


void SomaSoundAlsa::setParams( int slot )
{
  SoundSlot & sound = soundBank().sound( slot );
  const WavHeader &hdr = sound.compressinfo.hdr;
  d->nchannels = hdr.channels;
  d->sampleSize = hdr.sampleSize;
  d->signedSamples = hdr.sign;
  freqDsp = hdr.rate;
  stop();
  init();
}


void SomaSoundAlsa::loadSounds()
{
  if( !ok )
    return;

  // FIXME: too early, the bank is not here yet !
  soundBank().init();
  soundBank().loadSounds();
}


void SomaSoundAlsa::process( int type )
{
  pthread_mutex_lock( &listLock );

  unsigned nsnd = soundBank().sounds().size();
  if( (unsigned) type >= nsnd || !ok )
  {
    pthread_mutex_unlock( &listLock );
    return;
  }

  if( _inuse.size() <= (unsigned) type )
  {
    _inuse.reserve( nsnd );
    _inuse.insert( _inuse.end(), nsnd - _inuse.size(), 0 );
  }

  unsigned      n = _inuse[ type ];
  if( n >= MaxSameSample )
    stopOld( type );    // too many sounds at the same time: stop one of them
  SoundIterator *iter
    = SoundIterator::makeIterator( soundBank().sound( type ) );
  if( n > 0 )   // this one already plays : setup a little delay
  {
    int       r = rand() & 0x7ff;     // up to about 0.1 s
    iter->seek( -r );
  }
  jobs.push_back( SndReq( type ) );
  jobs.back().iterator = iter;
  ++_inuse[ type ];
  //cout << "jeu unlock\n";
  pthread_mutex_unlock( &listLock );

  if( !threadRunning )
    {
      threadRunning = true;
      //cout << "creation thread.\n";
      pthread_create( &updateThrd, 0, updateThread, this );
    }
}


void * SomaSoundAlsa::updateThread( void * sp )
{
  ((SomaSoundAlsa *) sp)->update();

  return( 0 );
}


void SomaSoundAlsa::update()
{
  list<SndReq>::iterator        ij, fj, itmp;
  unsigned                      sz, i, smp, ns;
  int                           snd, pos;

  // cout << "thread start\n";

  sz = bufferSize;      // write series of buffer size (synchro)

  snd_pcm_sframes_t frames;

  pthread_mutex_lock( &listLock );
  //cout << "thread lock\n" << flush;
  unsigned nsnd = soundBank().sounds().size();
  _inuse.reserve( nsnd );
  if( _inuse.size() < nsnd )
    _inuse.insert( _inuse.end(), nsnd - _inuse.size(), 0 );
  for( i=0; i<nsnd; ++i )
    _inuse[i] = 0;

  while( jobs.size() != 0 )
    {
      #if 0
      for( i=0; i<sz; ++i )       // pour chaque pas de temps
      {
        snd = 0;
        ns = 0;
        //dbgn = jobs.size();
        ij = jobs.begin();
        fj = jobs.end();
        while( ij!=fj )
          {
            pos = (*ij).pos + i;
            smp = (*ij).type;
            SoundSlot & sl = _sounds->sound( smp );
            if( sl.valid && sl.loaded && pos < (int) sl.buffer.size() )
              {
                if( pos >= 0 )  // if started
                  {
                    snd += sl.buffer[pos];
                    ++ns;
                  }
                ++ij;
              }
            else        // sample fini
              {
                itmp = ij;
                ++ij;
                --_inuse[ (*itmp).type ];
                jobs.erase( itmp );
                //fj = jobs.end();
                /*cout << "sample fini : " << dbgn << " -> " << jobs.size()
                  << ".\n";*/
              }
          }
        snd -= (ns-1) * 128;    // remettre le bon zÃ¯Â¿Â½ro
        if( snd < 0 )
          snd = 0;
        else if( snd > 255 )
          snd = 255;            // couper ce qui dÃ¯Â¿Â½passe
#if defined( sun ) || defined( __sun )
        snd -= 128;             // signÃ¯Â¿Â½ sur Sun
#endif

        buffer[i] = (unsigned char) snd;
      }

    for( ij=jobs.begin(), fj=jobs.end(); ij!=fj; ++ij )
      {
        (*ij).pos += sz;
        //cout << (*ij).pos << " ";
      }
#endif


      #if 1
      // cout << "sz: " << sz << ", jobs: " << jobs.size() << endl;
      for( i=0; i<sz; ++i )     // pour chaque pas de temps
        {
          snd = 0;
          ns = 0;
          //dbgn = jobs.size();
          ij = jobs.begin();
          fj = jobs.end();
          while( ij!=fj )
            {
              SoundIterator & iter = *ij->iterator;
              pos = (int) iter.pos();
              smp = (*ij).type;
              if( iter.sound().valid && pos < (int) iter.size() )
              {
                if( pos >= 0 )        // if started
                {
                  snd += (unsigned char) iter.getch();
                  ++ns;
                }
                else
                  iter.getch();
                ++ij;
              }
              else      // sample fini
              {
                itmp = ij;
                ++ij;
                --_inuse[ (*itmp).type ];
                jobs.erase( itmp );
                //fj = jobs.end();
                /*cout << "sample fini : " << dbgn << " -> " << jobs.size()
                  << ".\n";*/
              }
            }
          snd -= (ns-1) * 128;  // shift for zero
          if( snd < 0 )
            snd = 0;
          else if( snd > 255 )
            snd = 255;          // cut overflows

          buffer[i] = (unsigned char) snd;
        }
      #endif

      // cout << "thread unlock (joue)\n" << flush;
      pthread_mutex_unlock( &listLock );
      // cout << "OK\n" << flush;

      // force playing sound, wait for it to finish (without taking CPU)
      // la synchro est faite automatiquement par la taille des buffers...
      //cout << "waiting...\n";
      //snd_pcm_wait( d->handle, -1 );
      int done = 0;
      while( done < (int) sz )
      {
        frames = snd_pcm_writei( d->handle, buffer + done, sz - done );  // play
        // cout << "frames: " << frames << " / " << sz - done << endl;
        if( frames < 0 )
        {
          cerr << "snd_pcm_writei <0: " << snd_strerror(frames) << endl;
          frames = snd_pcm_recover( d->handle, frames, 1 );
          cout << "recover: " << frames << endl;
        }
        if( frames < 0 )
        {
          cerr << "snd_pcm_writei failed: " << snd_strerror(frames) << endl;
          break;
        }
        else
        {
          done += frames;
        }
      }
//       if( frames > 0 && (unsigned) frames < sz )
//         cerr << "Short write (expected " << sz << ", wrote " << frames
//           << ")\n";

      //cout << "thread lock\n" << flush;
      pthread_mutex_lock( &listLock );
      //cout << "OK\n" << flush;
    }

  pthread_mutex_unlock( &listLock );
  //cout << "thread unlock\n" << flush;

  // snd_pcm_wait( d->handle, -1 ); // we will stop playing for a while

  //cout << "thread destroyed.\n";
  threadRunning = false;
}


void SomaSoundAlsa::stop()
{
  unsigned      i;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stop)\n" << flush;
  jobs.erase( jobs.begin(), jobs.end() );
  for( i = 0; i<_inuse.size(); ++i )
    _inuse[ i ] = 0;
  //cout << "jeu unlock (stop)\n" << flush;
  pthread_mutex_unlock( &listLock );
/*  if( ok )
    {
      snd_pcm_wait( d->handle, 2000 );
    }*/
}


void SomaSoundAlsa::stop( int type )
{
  if( !ok )
    return;

  list<SndReq>::iterator        ij, fj=jobs.end(), tj;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stop 1)\n" << flush;
  if( _inuse.size() > (unsigned) type && _inuse[ type ] )
    {
      for( ij=jobs.begin(); ij!=fj; ++ij )
        if( (*ij).type == type )
          {
            tj = ij;
            --ij;
            jobs.erase( tj );
          }
      _inuse[ type ] = 0;
    }
  // cout << "jeu unlock (stop 1, " << (int) type << ")\n" << flush;
  pthread_mutex_unlock( &listLock );
}


void SomaSoundAlsa::stopOld( int type )
{
  list<SndReq>::iterator        ij, fj=jobs.end(), tj;
  bool                          fst = true;
  int                           maxpos = -10000;

  //pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stopOld)\n" << flush;
  if( _inuse.size() > (unsigned) type && _inuse[ type ] )
  {
      for( ij=jobs.begin(); ij!=fj; ++ij )
        if( (*ij).type == type )
          {
            SoundIterator::size_type ipos = ij->iterator->pos();
            if( fst || ipos > maxpos )
              {
                fst = false;
                tj = ij;
                maxpos = ipos;
              }
          }
      if( !fst )
        {
          jobs.erase( tj );
          --_inuse[ type ];
        }
    }
  //cout << "jeu unlock (stopOld)\n" << flush;
  //pthread_mutex_unlock( &listLock );
}


void SomaSoundAlsa::close()
{
  stop();
  if( ok )
    {
      int err;
      if( ( err = snd_pcm_close( d->handle ) ) < 0 )
        cerr << "Error while closing ALSA device : " << snd_strerror(err);
    }
}


unsigned SomaSoundAlsa::inuse( int type )
{
  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (inuse)\n" << flush;
  unsigned num;
  if( _inuse.size() <= (unsigned) type )
    num = 0;
  else
    num = _inuse[ type ];
  //cout << "jeu unlock (inuse)\n" << flush;
  pthread_mutex_unlock( &listLock );

  return( num );
}


string SomaSoundAlsa::name() const
{
  return "ALSA";
}


float SomaSoundAlsa::priorityRating() const
{
  return 150;
}


#endif

