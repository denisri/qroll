/***************************************************************************
                          dsp.cc
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2006 by Denis Rivi�re
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


#ifdef RR_DSP

#include <roll/sound/dsp.h>
#include <roll/struct/general.h>
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

using namespace std;


// namespace
// {
// 
//   bool dummy()
//   {
//     new RRDspSound;
//     return true;
//   }
// 
//   bool	x = dummy();
// }


const unsigned		RRDspSound::MaxSameSample = 3;


RRDspSound::RRDspSound() : fd( 0 ), ok( true ), bufferSize( 0 ), buffer( 0 ), 
  updateThrd( 0 ), threadRunning( false )
{
  init();
}


RRDspSound::~RRDspSound()
{
  close();
  delete[] buffer;
  for( int i=0; i<NO_SOUND; ++i )
    delete[] sounds[i];
}


void RRDspSound::init()
{
  ok = true;
  RRSoundProcessor::init();

#if defined( sun ) || defined( __sun )
#define	QR_SOUNDDEV	"/dev/audio"
#else
#ifdef linux
#define QR_SOUNDDEV	"/dev/dsp"
#endif
#endif

  int	i;

  for( i=0; i<NO_SOUND; ++i )
    {
      sounds[i] = 0;
      sndLen[i] = 0;
    }

#ifdef QR_SOUNDDEV
  cout << "Sound device : " << QR_SOUNDDEV << endl;

  // Open the soundcard device.
  if ( (fd = open( QR_SOUNDDEV, O_WRONLY)) < 0 )
  {
    cerr << "Failed to open " << QR_SOUNDDEV << "\n";
    ok = false;
    return;
  }

#ifdef linux	// sp�cifique...
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
  cout << "buffer size vis� : " << bufferSize << endl;

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
  cout << QR_SOUNDDEV << " operations are experimental: sound params may not " 
  <<"be set correctly, and sound are likely to be awful lound noise.\n";*/

  // on essaie quand-m�me...

  //	Pourquoi faut-il demander 11 kHz pour obtenir 22 ????????
  freqDsp = 11025;				// 22.05 kHz
  bufferSize = 512;				// buffer de 512 octets

  audio_info_t	auinf;

  /* debug
  AUDIO_INITINFO( &auinf );
  if( ioctl( fd, AUDIO_SETINFO, &auinf ) == -1 ) // �crit les nouvelles valeurs
    {
      cerr << "getinfo foir�\n";
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
  // est-ce n�cessaire de remplir �a ?
  //auinf.play.avail_ports = AUDIO_SPEAKER | AUDIO_HEADPHONE | AUDIO_LINEOUT;
  auinf.play.buffer_size = bufferSize;
  auinf.play.balance = AUDIO_MID_BALANCE;

  if( ioctl( fd, AUDIO_SETINFO, &auinf ) == -1 ) // �crit les nouvelles valeurs
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

#else	// pas sun non plus: device non reconnu
  cout << "*** WARNING ***\n";
  cout << QR_SOUNDDEV << " operations not recognized: sound params will not " 
       <<"be set, and sound are likely to be awful lound noise.\n";

  freqDsp = 8000;	// par d�faut �a doit �tre 8 kHz... (?)
  bufferSize = 512;
#endif
#endif

  delete[] buffer;
  buffer = new unsigned char[ bufferSize ];
  listLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

  loadSounds();

#else	// ifdef QR_SOUNDDEV
  cout << "No sound device defined for this system, QRoll will be mute. "
       << "Sorry...\n";
  ok = false;
#endif
}


void RRDspSound::loadSounds()
{
  if( !ok )
    return;

  int	i;
  int	f;
  string	fnbase;
  string	filename;
  struct stat	fst;
  ssize_t	rd;

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
            const unsigned	whl = 44;	// .wav header length
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


void RRDspSound::process( SNDLIST type )
{
  if( type >= NO_SOUND || !ok )
    return;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock\n";
  unsigned	n = _inuse[ type ];
  if( n >= MaxSameSample )
    stopOld( type );	// trop de sons en m�me temps: arr�te-en un
  if( n > 0 )	// d�j� ce son : mettre un petit d�callage
    {
      int	r = rand() & 0x7ff;	// jusqu'� 0.1 s environ
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


void * RRDspSound::updateThread( void * sp )
{
  ((RRDspSound *) sp)->update();

  return( 0 );
}


void RRDspSound::update()
{
  list<SndReq>::iterator	ij, fj, itmp;
  unsigned			sz, i, smp, ns;
  int				snd, pos;

  //cout << "thread start\n";

  /* sz = (unsigned) ( freqDsp * 0.05 );	// pas de 1/20 seconde environ
  if( (int) sz > bufferSize )
    sz = bufferSize;			// limit� aussi par la taille du buffer
  */

  sz = bufferSize;	// �crire des s�ries de la taille du buffer (synchro)

  //unsigned dbgn;

  pthread_mutex_lock( &listLock );
  //cout << "thread lock\n" << flush;

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
	      if( pos < (int) sndLen[smp] )
		{
		  if( pos >= 0 )	// si commenc�
		    {
		      snd += sounds[smp][pos];
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
	  snd -= (ns-1) * 128;	// remettre le bon z�ro
	  if( snd < 0 )
	    snd = 0;
	  else if( snd > 255 )
	    snd = 255;		// couper ce qui d�passe
#if defined( sun ) || defined( __sun )
	  snd -= 128;		// sign� sur Sun
#endif

	  buffer[i] = (unsigned char) snd;
	}

      // remettre � jour les positions des samples

      for( ij=jobs.begin(), fj=jobs.end(); ij!=fj; ++ij )
	{
	  (*ij).pos += sz;
	  //cout << (*ij).pos << " ";
	}
      //cout << jobs.size() << " samples, sz= " << sz << "\n";

      // c'est hyper pas du tout optimis� cette routine...

      //cout << "thread unlock (joue)\n" << flush;
      pthread_mutex_unlock( &listLock );
      //cout << "OK\n" << flush;

      // force � jouer, attends que ce soit fini (sans prendre du CPU)
      //ioctl( fd, SNDCTL_DSP_SYNC );	// �a marche PLUS �a !!!!
      // la synchro est faite automatiquement par la taille des buffers...
      write( fd, buffer, sz );	// joue

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
  ioctl( fd, SNDCTL_DSP_POST );	// on va s'arr�ter de jouer pendant un moment
#else
#if defined( sun ) || defined( __sun )
  ioctl( fd, AUDIO_DRAIN );
#endif
#endif

  //cout << "thread d�truit.\n";
  threadRunning = false;
}


void RRDspSound::stop()
{
  unsigned	i;

  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (stop)\n" << flush;
  jobs.erase( jobs.begin(), jobs.end() );
  for( i = 0; i<NO_SOUND; ++i )
    _inuse[ i ] = 0;
  //cout << "jeu unlock (stop)\n" << flush;
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


void RRDspSound::stop( SNDLIST type )
{
  if( !ok )
    return;

  list<SndReq>::iterator	ij, fj=jobs.end(), tj;

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


void RRDspSound::stopOld( SNDLIST type )
{
  list<SndReq>::iterator	ij, fj=jobs.end(), tj;
  bool				fst = true;
  int				maxpos = -10000;

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


void RRDspSound::close()
{
  stop();
  if( ok )
    {
      if( ::close( fd ) )
	cerr << "Error while closing /dev/dsp (?\?)\n";
    }
}


unsigned RRDspSound::inuse( SNDLIST type )
{
  pthread_mutex_lock( &listLock );
  //cout << "jeu lock (inuse)\n" << flush;
  unsigned num = _inuse[ type ];
  //cout << "jeu unlock (inuse)\n" << flush;
  pthread_mutex_unlock( &listLock );

  return( num );
}


string RRDspSound::name() const
{
  return "DSP (linux or solaris systems)";
}


float RRDspSound::priorityRating() const
{
  return 100;
}


#endif

