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


#ifdef RR_ALSA

#include <roll/sound/alsaProcessor.h>
#include <roll/struct/general.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;


const unsigned          RRAlsaSound::MaxSameSample = 3;


struct RRAlsaSound::Private
{
  Private();

  snd_pcm_t *handle;
};


RRAlsaSound::Private::Private()
  : handle( 0 )
{
}


RRAlsaSound::RRAlsaSound() : ok( true ), bufferSize( 0 ), buffer( 0 ),
  updateThrd( 0 ), threadRunning( false ), freqDsp( 22050 ), d( new Private )
{
  init();
}


RRAlsaSound::~RRAlsaSound()
{
  close();
  delete[] buffer;
  for( int i=0; i<NO_SOUND; ++i )
    delete[] sounds[i];
  delete d;
}


void RRAlsaSound::init()
{
  ok = true;
  RRSoundProcessor::init();

#define QR_SOUNDDEV     "default"

  int   i;

  for( i=0; i<NO_SOUND; ++i )
    {
      sounds[i] = 0;
      sndLen[i] = 0;
    }

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
  d->handle = handle;

  // mono, 8 bits, 22 kHz
  if( ( err = snd_pcm_set_params( handle, SND_PCM_FORMAT_U8,
    SND_PCM_ACCESS_RW_INTERLEAVED, 1, freqDsp, 1, 50000 ) ) < 0 )
  {
    cerr << "ALSA error: " << snd_strerror(err) << endl;
    ok = false;
    snd_pcm_close( handle );
    return;
  }

  snd_pcm_hw_params_t *hwparams = 0;
  snd_pcm_hw_params_alloca( &hwparams ); // will be automatically freed
  err = snd_pcm_hw_params_any( handle, hwparams );
  if( err < 0 )
  {
    cerr << "Broken configuration for playback: no configurations available: "
      << snd_strerror(err) << endl;
    ok = false;
    snd_pcm_close( handle );
    return;
  }
  unsigned bufferTime = 25000;    // 1/40 de seconde en 8 bits mono
  int dir = 1;

  err = snd_pcm_hw_params_set_buffer_time_near( handle, hwparams, &bufferTime,
                                                &dir);
  if (err < 0)
  {
    cerr << "Unable to set buffer time " << bufferTime << " for playback: "
      << snd_strerror(err) << endl;
    snd_pcm_close(handle);
    ok = false;
    return;
  }
  cout << "buffer time: " << bufferTime << endl;

  snd_pcm_uframes_t bufsz = 0;
  err = snd_pcm_hw_params_get_buffer_size( hwparams, &bufsz );
  if (err < 0)
  {
    cerr << "Unable to get buffer size for playback: "
      << snd_strerror(err) << endl;
    // why doesn't this work ???
    bufsz = bufferTime * freqDsp / 1000000;
/*    snd_pcm_close(handle);
    ok = false;
    return;*/
  }
  bufferSize = bufsz;
/*  if( bufferSize == 551 ) // why why why why ??
    bufferSize = 550;*/
  cout << "obtained buffer size : " << bufferSize << endl;

  delete[] buffer;
  buffer = new unsigned char[ bufferSize ];
  listLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

  loadSounds();
}


void RRAlsaSound::loadSounds()
{
  if( !ok )
    return;

  int   i;
  int   f;
  string        fnbase;
  string        filename;
  struct stat   fst;
  ssize_t       rd;

  fnbase = roll::qRollSharePath() + "/sounds/";

  for( i=0; i<NO_SOUND; ++i )
    if( !sounds[i] )
      {
        filename = fnbase + _sndFile[ i ] + ".wav";
        f = open( filename.c_str(), O_RDONLY );
        if( f == -1 || fstat( f, &fst ) != 0 )
          {
            cerr << "unable to load sample " << filename << ".\n";
          }
        else
          {
            const unsigned      whl = 44;       // .wav header length
            sndLen[i] = fst.st_size - whl;
            sounds[i] = new unsigned char[ sndLen[i] ];
            lseek( f, whl, SEEK_SET );
            rd = read( f, sounds[i], sndLen[i] );
            if( rd != (ssize_t) sndLen[i] )
              {
                cerr << filename << " : only " << rd << " bytes read, "
                     << (int) sndLen[i] << " expected !\n";
                sndLen[i] = rd;
              }
            ::close( f );
            // cout << filename << " read, " << rd << " bytes.\n";
          }
      }
}


void RRAlsaSound::process( SNDLIST type )
{
  if( type >= NO_SOUND || !ok )
    return;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock\n";
  unsigned      n = _inuse[ type ];
  if( n >= MaxSameSample )
    stopOld( type );    // too many sounds at the same time: stop one of them
  if( n > 0 )   // this one already plays : setup a little delay
    {
      int       r = rand() & 0x7ff;     // up to about 0.1 s
      jobs.push_back( (SndReq) { type, -r } );
    }
  else
    jobs.push_back( (SndReq) { type, 0 } );
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


void * RRAlsaSound::updateThread( void * sp )
{
  ((RRAlsaSound *) sp)->update();

  return( 0 );
}


void RRAlsaSound::update()
{
  list<SndReq>::iterator        ij, fj, itmp;
  unsigned                      sz, i, smp, ns;
  int                           snd, pos;

  //cout << "thread start\n";

  /* sz = (unsigned) ( freqDsp * 0.05 );        // pas de 1/20 seconde environ
  if( (int) sz > bufferSize )
    sz = bufferSize;                    // limited bu buffer size too
  */

  sz = bufferSize;      // write series of buffer size (synchro)

  snd_pcm_sframes_t frames;

  pthread_mutex_lock( &listLock );
  //cout << "thread lock\n" << flush;

  while( jobs.size() != 0 )
    {
      for( i=0; i<sz; ++i )     // pour chaque pas de temps
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
              if( pos < (int) sndLen[smp] )
                {
                  if( pos >= 0 )        // if started
                    {
                      snd += sounds[smp][pos];
                      ++ns;
                    }
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

      // update samples positions

      for( ij=jobs.begin(), fj=jobs.end(); ij!=fj; ++ij )
        {
          (*ij).pos += sz;
          //cout << (*ij).pos << " ";
        }
      //cout << jobs.size() << " samples, sz= " << sz << "\n";

      // c'est hyper pas du tout optimisé cette routine...

      //cout << "thread unlock (joue)\n" << flush;
      pthread_mutex_unlock( &listLock );
      //cout << "OK\n" << flush;

      // force playing sound, wait for it to finish (without taking CPU)
      // la synchro est faite automatiquement par la taille des buffers...
      //cout << "waiting...\n";
      //snd_pcm_wait( d->handle, -1 );
      int done = 0;
      while( done < sz )
      {
        frames = snd_pcm_writei( d->handle, buffer + done, sz - done );  // play
//       cout << "frames: " << frames << endl;
        if( frames < 0 )
          frames = snd_pcm_recover( d->handle, frames, 1 );
        else
        {
          done += frames;
        }
/*        if( frames < 0 )
        {
          cerr << "snd_pcm_writei failed: " << snd_strerror(frames) << endl;
          break;
        }*/
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


void RRAlsaSound::stop()
{
  unsigned      i;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stop)\n" << flush;
  jobs.erase( jobs.begin(), jobs.end() );
  for( i = 0; i<NO_SOUND; ++i )
    _inuse[ i ] = 0;
  //cout << "jeu unlock (stop)\n" << flush;
  pthread_mutex_unlock( &listLock );
/*  if( ok )
    {
      snd_pcm_wait( d->handle, 2000 );
    }*/
}


void RRAlsaSound::stop( SNDLIST type )
{
  if( !ok )
    return;

  list<SndReq>::iterator        ij, fj=jobs.end(), tj;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stop 1)\n" << flush;
  if( _inuse[ type ] )
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


void RRAlsaSound::stopOld( SNDLIST type )
{
  list<SndReq>::iterator        ij, fj=jobs.end(), tj;
  bool                          fst = true;
  int                           maxpos = -10000;

  //pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stopOld)\n" << flush;
  if( _inuse[ type ] )
    {
      for( ij=jobs.begin(); ij!=fj; ++ij )
        if( (*ij).type == type )
          {
            if( fst || (*ij).pos > maxpos )
              {
                fst = false;
                tj = ij;
                maxpos = (*ij).pos;
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


void RRAlsaSound::close()
{
  stop();
  if( ok )
    {
      int err;
      if( ( err = snd_pcm_close( d->handle ) ) < 0 )
        cerr << "Error while closing ALSA device : " << snd_strerror(err);
    }
}


unsigned RRAlsaSound::inuse( SNDLIST type )
{
  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (inuse)\n" << flush;
  unsigned num = _inuse[ type ];
  //cout << "jeu unlock (inuse)\n" << flush;
  pthread_mutex_unlock( &listLock );

  return( num );
}


string RRAlsaSound::name() const
{
  return "ALSA";
}


float RRAlsaSound::priorityRating() const
{
  return 150;
}


#endif

