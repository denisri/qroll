/***************************************************************************
                          dsp.cc
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2006 by Denis Rivière
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


#ifdef SOMA_SOUND_OSS

#include "somasoundoss.h"
#include "soundbank.h"
#include "soundslot.h"
#include <sys/ioctl.h>
#ifdef linux
#include <sys/soundcard.h>
#else
#if defined( __sun ) || defined( sun )
#include <sys/audioio.h>
#include <cerrno>
#endif
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace soma;
using namespace std;


const unsigned		SomaSoundOSS::MaxSameSample = 3;


SomaSoundOSS::SomaSoundOSS()
  : SomaSoundProcessor(), fd( 0 ), ok( true ), bufferSize( 0 ), buffer( 0 ),
    updateThrd( 0 ), threadRunning( false )
{
  init();
}


SomaSoundOSS::~SomaSoundOSS()
{
  close();
  delete[] buffer;
}


void SomaSoundOSS::init()
{
  ok = true;
  SomaSoundProcessor::init();

#if defined( sun ) || defined( __sun )
#define SOMA_SOUND_OSS_SOUNDDEV "/dev/audio"
#else
#ifdef linux
#define SOMA_SOUND_OSS_SOUNDDEV "/dev/dsp"
#endif
#endif

#ifdef SOMA_SOUND_OSS_SOUNDDEV
  cout << "OSS Sound device : " << SOMA_SOUND_OSS_SOUNDDEV << endl;

  // Open the soundcard device.
  if ( (fd = open( SOMA_SOUND_OSS_SOUNDDEV, O_WRONLY)) < 0 )
  {
    cerr << "Failed to open " << SOMA_SOUND_OSS_SOUNDDEV << "\n";
    ok = false;
    return;
  }

#ifdef linux	// specific...
  // mono, 8 bits, 22 kHz
  int	arg = AFMT_U8;
  if( ioctl( fd, SNDCTL_DSP_SETFMT /*SOUND_PCM_WRITE_BITS*/, &arg ) == -1 )
    {
      cerr << "Error while setting SNDCTL_DSP_SETFMT\n";
      //SOUND_PCM_WRITE_BITS\n";
      ok = false;
      ::close( fd );
    }
  if (arg != 8)
    {
      cerr << "Failed to set sample size to unsigned 8 bits.\n";
      ok = false;
      ::close( fd );
    }
  if( !ok )
    return;

  arg = 0;
  if( ioctl( fd, SNDCTL_DSP_STEREO, &arg ) == -1 )
    {
      cerr << "Error while setting SNDCTL_DSP_STEREO\n";
      ok = false;
      ::close( fd );
    }
  if (arg != 0)
    {
      cerr << "This device doesn't support mono mode!?!\n";
      ok = false;
      ::close( fd );
    }
  if( !ok )
    return;

  arg = 22050;
  freqDsp = 22050;
  if( ioctl( fd, SNDCTL_DSP_SPEED /*SOUND_PCM_WRITE_RATE*/, &arg ) == -1 )
    {
      cerr << "Error while setting SNDCTL_DSP_SPEED to 22kHz\n";
      ok = false;
      ::close( fd );
      return;
    }

  bufferSize = freqDsp / 40;	// 1/40 de seconde en 8 bits mono
  // en exposant de 2
  unsigned	fragments = (unsigned) ceil( log( (double) bufferSize ) 
					     / log( (double) 2 ) + 0.5 );
  bufferSize = 1U << (fragments - 1);	// modif au plus proche
  cout << "buffer size visÃ¯Â¿Â½ : " << bufferSize << endl;

  // 0xMMMMSSSS, MMMM = nb de fragments (2), SSSS = taille en exposant de 2
  fragments |= 0x20000;
  if( ioctl( fd, SNDCTL_DSP_SETFRAGMENT, &fragments ) == -1 )
    {
      cerr << "Couldn't set buffers size and numbers\n";
      ok = false;
      ::close( fd );
      return;
    }
  bufferSize = 1U << ( ( fragments & 0xFFFF ) - 1 );
  cout << "buffer size obtenu : " << bufferSize << endl;


#else	// pas linux
#if defined( sun ) || defined( __sun )
  /*cout << "*** WARNING ***\n";
  cout << SOMA_SOUND_OSS_SOUNDDEV
  << " operations are experimental: sound params may not "
  <<"be set correctly, and sound are likely to be awful lound noise.\n";*/

  //	Pourquoi faut-il demander 11 kHz pour obtenir 22 ????????
  freqDsp = 11025;				// 22.05 kHz
  bufferSize = 512;				// buffer de 512 octets

  audio_info_t	auinf;

  /* debug
  AUDIO_INITINFO( &auinf );
  if( ioctl( fd, AUDIO_SETINFO, &auinf ) == -1 ) // Ã¯Â¿Â½crit les nouvelles valeurs
    {
      cerr << "getinfo foirÃ¯Â¿Â½\n";
    }
  cout << "avant modif : freq : " << auinf.play.sample_rate 
       << ", precision : " 
       << auinf.play.precision << ", encoding : " << auinf.play.encoding 
       << ", buffsize : " << auinf.play.buffer_size << endl;
       // end debug*/

  AUDIO_INITINFO( &auinf );

  auinf.output_muted = 0;			// non mute
  auinf.play.sample_rate = freqDsp;
  auinf.play.channels = 1;
  auinf.play.precision = 8;			// samples de 8 bits
  auinf.play.encoding = AUDIO_ENCODING_LINEAR;	// 8 bits unsigned
  auinf.play.gain = AUDIO_MAX_GAIN / 2;		// volume max
  //auinf.play.port = AUDIO_SPEAKER;		// sortie haut-parleur interne
  // is it necessary to fill this ?
  //auinf.play.avail_ports = AUDIO_SPEAKER | AUDIO_HEADPHONE | AUDIO_LINEOUT;
  auinf.play.buffer_size = bufferSize;
  auinf.play.balance = AUDIO_MID_BALANCE;

  if( ioctl( fd, AUDIO_SETINFO, &auinf ) == -1 ) // write new values
    {
      if( errno == EINVAL )
        cerr << "EINVAL\n";
      else if( errno == EBUSY )
        cerr << "EBUSY\n";
      ok = false;
      cerr << "Failed to set audio params.\n";
      ::close( fd );
    }
  /*cout << "freq : " << auinf.play.sample_rate << ", precision : " 
       << auinf.play.precision << ", encoding : " << auinf.play.encoding 
       << ", buffsize : " << auinf.play.buffer_size << endl;*/

#else	// not sun either plus: unknown device
  cout << "*** WARNING ***\n";
  cout << SOMA_SOUND_OSS_SOUNDDEV
    << " operations not recognized: sound params will not "
    <<"be set, and sound are likely to be awful lound noise.\n";

  freqDsp = 8000;	// by default, assume 8 kHz... (?)
  bufferSize = 512;
#endif
#endif

  delete[] buffer;
  buffer = new unsigned char[ bufferSize ];
  listLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

  loadSounds();

#else	// ifdef SOMA_SOUND_OSS_SOUNDDEV
  cout << "No sound device defined for this system, OSS sound will be mute. "
       << "Sorry...\n";
  ok = false;
#endif
}


void SomaSoundOSS::loadSounds()
{
  if( !ok )
    return;

  // FIXME: too early, the bank is not here yet !
  soundBank().init();
  soundBank().loadSounds();
}


void SomaSoundOSS::process( int type )
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

  unsigned	n = _inuse[ type ];
  if( n >= MaxSameSample )
    stopOld( type );   // too many sounds at the same time: stop one of them
  if( n > 0 )  // this one already plays : setup a little delay
  {
    int	r = rand() & 0x7ff;	 // up to about 0.1 s
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


void * SomaSoundOSS::updateThread( void * sp )
{
  ((SomaSoundOSS *) sp)->update();

  return( 0 );
}


void SomaSoundOSS::update()
{
  list<SndReq>::iterator	ij, fj, itmp;
  unsigned			sz, i, smp, ns;
  int				snd, pos;

  //cout << "thread start\n";

  sz = bufferSize;	// write series of buffer size (synchro)

  //unsigned dbgn;

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
    for( i=0; i<sz; ++i )	// pour chaque pas de temps
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
                if( pos >= 0 )	// if started
                  {
                    snd += sl.buffer[pos];
                    ++ns;
                  }
                ++ij;
              }
            else	// sample fini
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
        snd -= (ns-1) * 128;	// remettre le bon zÃ¯Â¿Â½ro
        if( snd < 0 )
          snd = 0;
        else if( snd > 255 )
          snd = 255;		// couper ce qui dÃ¯Â¿Â½passe
#if defined( sun ) || defined( __sun )
        snd -= 128;		// signÃ¯Â¿Â½ sur Sun
#endif

        buffer[i] = (unsigned char) snd;
      }

    // remettre Ã¯Â¿Â½ jour les positions des samples

    for( ij=jobs.begin(), fj=jobs.end(); ij!=fj; ++ij )
      {
        (*ij).pos += sz;
        //cout << (*ij).pos << " ";
      }
    //cout << jobs.size() << " samples, sz= " << sz << "\n";

    // c'est hyper pas du tout optimisÃ¯Â¿Â½ cette routine...

    //cout << "thread unlock (joue)\n" << flush;
    pthread_mutex_unlock( &listLock );
    //cout << "OK\n" << flush;

    // force playing, waiting for it to finish without taking CPU
    //ioctl( fd, SNDCTL_DSP_SYNC );	// doesn't work anymore...
    // synchro is performed automatically via the buffer sizes...
    write( fd, buffer, sz );	// play it

#if defined( sun ) || defined( __sun )
    ioctl( fd, AUDIO_DRAIN );
#endif

    //cout << "thread lock\n" << flush;
    pthread_mutex_lock( &listLock );
    //cout << "OK\n" << flush;
  }

  pthread_mutex_unlock( &listLock );
  //cout << "thread unlock\n" << flush;

#ifdef linux
  ioctl( fd, SNDCTL_DSP_POST );	// will stop playing for a while
#else
#if defined( sun ) || defined( __sun )
  ioctl( fd, AUDIO_DRAIN );
#endif
#endif

  //cout << "thread destroyed.\n";
  threadRunning = false;
}


void SomaSoundOSS::stop()
{
  unsigned	i;

  pthread_mutex_lock( &listLock );
  jobs.erase( jobs.begin(), jobs.end() );
  for( i = 0; i<_inuse.size(); ++i )
    _inuse[ i ] = 0;
  pthread_mutex_unlock( &listLock );
  if( ok )
    {
#ifdef linux
      ioctl( fd, SNDCTL_DSP_SYNC );
#else
#if defined( sun ) || defined( __sun )
      ioctl( fd, AUDIO_DRAIN );
#endif
#endif
    }
}


void SomaSoundOSS::stop( int type )
{
  if( !ok )
    return;

  list<SndReq>::iterator	ij, fj=jobs.end(), tj;

  pthread_mutex_lock( &listLock );
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
  pthread_mutex_unlock( &listLock );
}


void SomaSoundOSS::stopOld( int type )
{
  list<SndReq>::iterator	ij, fj=jobs.end(), tj;
  bool				fst = true;
  int				maxpos = -10000;

  //pthread_mutex_lock( &listLock );
  if( _inuse.size() > (unsigned) type && _inuse[ type ] )
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
  //pthread_mutex_unlock( &listLock );
}


void SomaSoundOSS::close()
{
  stop();
  if( ok && ::close( fd ) )
    cerr << "Error while closing " << SOMA_SOUND_OSS_SOUNDDEV << " (?\?)\n";
}


unsigned SomaSoundOSS::inuse( int type )
{
  pthread_mutex_lock( &listLock );
  unsigned num;
  if( _inuse.size() <= (unsigned) type )
    num = 0;
  else
    num = _inuse[ type ];
  pthread_mutex_unlock( &listLock );

  return( num );
}


string SomaSoundOSS::name() const
{
  return "OSS (linux or solaris systems)";
}


float SomaSoundOSS::priorityRating() const
{
  return 100;
}


#endif

