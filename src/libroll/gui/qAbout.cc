/***************************************************************************
                          qAbout.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2005 by Denis Rivi�e
    email                : nudz@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <roll/gui/qAbout.h>
#include <roll/gui/mainWin.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qnamespace.h>
#include <qsound.h>
#include <qevent.h>

#include <roll/game/vars.h>
#include <roll/sound/dsp.h>

#include <fstream>
#include <stdexcept>

#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <pthread.h>
#ifdef linux
#include <sys/soundcard.h>
#else	// linux
#if defined( sun ) || defined( __sun )
#include <sys/audioio.h>
#endif	// sun
#endif	// linux
#endif	// _WIN32

/* AudiQ classes sources - I directly include them so they don't appear
   externally
 */
#include "../sound/wavheader.cc"
#include "../sound/diffcode.cc"

using namespace roll;
using namespace std;
using namespace audiq;

namespace
{

  class QScrollingLabel : public QLabel
  {
  public:
    QScrollingLabel( QWidget* parent, const char* name = 0 );
    virtual ~QScrollingLabel();

    bool nextline;
    char *text;
    char *current;
    int  offset;
    int  speed;
    bool mustfill;

  protected:
    void paintEvent( QPaintEvent* );
    void showEvent( QShowEvent* );
    void resizeEvent( QResizeEvent* );
  };


  QScrollingLabel::QScrollingLabel( QWidget* parent, const char* name )
  : QLabel( parent, name ), nextline( false ), text( 0 ), current( 0 ),
  offset( 0 ), speed( 1 ), mustfill( true )
  {
  }


  QScrollingLabel::~QScrollingLabel()
  {
  }


  void QScrollingLabel::paintEvent( QPaintEvent* )
  {
    if( mustfill )
    {
      QPainter	paint( this );
      paint.setClipRect( 0, 0, width(), height() );
      paint.fillRect( 0, 0, width(), height(), QBrush( backgroundColor() ) );
      mustfill = false;
    }
    else if( text && nextline )
    {
      nextline = false;
      int	h = 16;
      int	n = speed;
      int	m = -2;

      int	x, y, w;
      x = 0;
      w = width();

      QPainter	paint( this );

      if( !current || current[0] == '\0' )
        current = text;
      char	*chr = strchr( current, '\n' );
      unsigned	l, of = 0;
      QFont	fnt = paint.font();

      while( current[of] == '/' )	// font code
      {
        ++of;
        switch( current[of] )
        {
          case 'b':	// bold
            fnt.setBold( true );
            ++of;
            break;
          case 'i':	// italic
            fnt.setItalic( true );
            ++of;
            break;
          case 'u':	// underline
            fnt.setUnderline( true );
            ++of;
            break;
          case 'h':	// height
            fnt.setPointSize( fnt.pointSize() + 2 );
            ++of;
            break;
          case 's':	// strikeout
            fnt.setStrikeOut( true );
            ++of;
            break;
          case 'f':	// fixed pitch
            fnt.setFixedPitch( true );
            ++of;
          default:	// not understood
            break;
        }
      }
      paint.setFont( fnt );
      QFontInfo	finf( fnt );	// I can't retreive the exact font height in
      h = int( fnt.pointSizeFloat() ) + 7;	// pixels !!
      //cout << "font size: " << h << endl;
      y = height()-h;

      if( chr )
        l = chr - current - of;
      else
        l = strlen( current + of );
      char	*line = new char[ l + 1 ];
      strncpy( line, current + of, l );
      line[ l ] = '\0';

      paint.setClipRect( x, y, w, h );
      paint.fillRect( x, height()-n, w, n,
                      QBrush( backgroundColor() ) );
      paint.drawText( x, height()+offset, w, h,
                      Qt::AlignHCenter | Qt::AlignBottom, line );
      delete[] line;
      paint.end();

      if( offset <= m-h )
      {
        offset = 0;
        if( chr )
          current += l+1+of;
        else
          current = text;
      }
      else
        offset -= n;
    }
  }


  void QScrollingLabel::showEvent( QShowEvent* )
  {
    mustfill = true;
  }


  void QScrollingLabel::resizeEvent( QResizeEvent* )
  {
    mustfill = true;
  }

}


struct QAbout::Private
{
  Private();

  QScrollingLabel	*edit;
  int			sndFD;
#if( !defined( _WIN32 ) && !defined( ABOUT_NO_SOUND ) )
  pthread_t		musThrd;
#endif
  bool			threadRunning;
  QSound		*qsound;
  bool			diffcoded;
  string		musicfile;
  string		tempfile;
};


QAbout::Private::Private()
  : sndFD( -1 ), qsound( 0 ), diffcoded( false )
{
}


QAbout::QAbout( QWidget* parent, const char* name ) 
  : QDialog( parent, name, true ), d( new Private )
{
  setCaption( tr( "About QRoll" ) );

  QVBoxLayout	*lay1 = new QVBoxLayout( this, 10, -1, "lay1" );
  d->edit = new QScrollingLabel( this, "edit" );
  d->edit->setLineWidth( 2 );
  d->edit->setMidLineWidth( 2 );
  d->edit->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  d->edit->setMinimumSize( 350, 250 );
  d->edit->setBackgroundColor( Qt::white );
#if QT_VERSION >= 0x040000
  d->edit->setAutoFillBackground( false );
  d->edit->setAttribute( Qt::WA_OpaquePaintEvent );
#endif

  QPushButton	*bok = new QPushButton( tr( "OK" ), this, "okbtn" );

  bok->setDefault( true );
  connect( bok, SIGNAL( clicked() ), this, SLOT( accept() ) );
  bok->setFixedSize( bok->sizeHint() );

  lay1->addWidget( d->edit );
  lay1->addSpacing( 10 );
  lay1->addWidget( bok );
  lay1->addSpacing( 10 );

  resize( 400, 400 );

  struct stat	buf;
  string	tname = RR_path + "/po/" + QRMainWin::language() 
    + "/about.txt";
  int		sres = stat( tname.c_str(), &buf );
  FILE	*	f = 0;

  if( sres == 0 )
    f = fopen( tname.c_str(), "r" );

  if( !sres && f )
    {
      d->edit->text = new char[ buf.st_size + 1 ];
      fread( d->edit->text, 1, buf.st_size, f );
      d->edit->text[ buf.st_size - 1 ] = '\0';
      fclose( f );
      d->edit->current = d->edit->text;

      QTimer	*tim = new QTimer( this, "timer" );
      connect( tim, SIGNAL( timeout() ), this, SLOT( nextLine() ) );
      tim->start( 25 );
    }
  else
    {
      QString	text = "\n\n\n\n";
      text += tr( "Cannot find about.txt file" ) + "\n\n\n";
      text += tr( "check config and ROLLDIR environment variable" );
      d->edit->setText( text );
    }

  d->musicfile = RR_path + "/data/music.adc";
  if( stat( d->musicfile.c_str(), &buf ) )
    d->musicfile = RR_path + "/data/music.wav";
  else
    d->diffcoded = true;
  d->tempfile = "/tmp/qroll_music.wav";

#if defined( linux ) || defined( ABOUT_NO_SOUND )
  bool enableQSound = false;
#else
  bool enableQSound = true;
#endif
  if( enableQSound && !QSound::isAvailable() )
    enableQSound = false;

  if( enableQSound )
    {
      d->threadRunning = false;
      string	file = d->musicfile;
      if( d->diffcoded )
        {
          file = d->tempfile;
          DiffCode::uncompress( d->musicfile, d->tempfile );
        }
      d->qsound = new QSound( file.c_str(), this );
      d->qsound->play();
    }
  else
    {
#if defined( ABOUT_NO_SOUND ) || defined( _WIN32 )
      d->threadRunning = false;
#else
      d->threadRunning = true;
      pthread_create( &d->musThrd, 0, musicThread, this );
#endif
    }
}


QAbout::~QAbout()
{
#ifndef ABOUT_NO_SOUND
  if( d->threadRunning )
    {
      d->threadRunning =false;
#ifndef _WIN32
      void	*garbage;
      pthread_join( d->musThrd, &garbage );
#endif	// _WIN32
    }
  if( d->sndFD >= 0 )
    {
      //	*** DSP ***
#ifdef __linux
      ioctl( d->sndFD, SNDCTL_DSP_RESET, (char*)0 );
#endif
      ::close( d->sndFD );
      d->sndFD = -1;

#ifdef RR_DSP
      RRDspSound	*dsp 
        = dynamic_cast<RRDspSound *>( &RRSoundProcessor::processor() );

      if( dsp )
        dsp->enable();
#endif // RR_DSP
    }
#endif // ABOUT_NO_SOUND

  if( d->qsound )
    {
      d->qsound->stop();
      delete d->qsound;
    }
  if( !d->tempfile.empty() )
    unlink( d->tempfile.c_str() );

  delete[] d->edit->text;
  d->edit->text = 0;

  delete d;
}


void QAbout::nextLine()
{
  if( d->edit->text )
  {
    int	n = d->edit->speed;
    d->edit->nextline = true;
    d->edit->scroll( 0, -n, QRect( 0, 0, d->edit->width(),
                     d->edit->height() ) );
  }
}


void * QAbout::musicThread( void* caller )
{
  ((QAbout *) caller)->music();
  return( 0 );
}


namespace
{

#if !defined( ABOUT_NO_SOUND ) && ( defined( __linux ) || defined( __sun ) )
  bool initSound( int fd, const WavHeader & hdr )
  {
#ifdef __linux	// specific...
    // mono, 8 bits, 22 kHz
    int arg = AFMT_U8;
    if( hdr.sampleSize == 2 )
      arg = AFMT_S16_LE;
    if( ioctl( fd, SNDCTL_DSP_SETFMT, (char*) &arg ) == -1 )
      {
        cerr << "Error while setting SNDCTL_DSP_SETFMT\n";
        //SOUND_PCM_WRITE_BITS\n";
        ::close( fd );
        return( false );
      }
    if( arg != hdr.sampleSize * 8 )
      {
        cerr << "Failed to set correct sample size.\n";
        ::close( fd );
        return( false );
      }

    arg = hdr.channels - 1;
    if( ioctl( fd, SNDCTL_DSP_STEREO, (char*)&arg ) == -1 )
      {
        cerr << "Error while setting SNDCTL_DSP_STEREO\n";
        ::close( fd );
        return( false );
      }
    if( arg != hdr.channels - 1 )
      {
        cerr << "This device doesn't support mono mode!?!\n";
        ::close( fd );
        return( false );
      }

    arg = hdr.rate;
    int	freqDsp = arg;
    if( ioctl( fd, SNDCTL_DSP_SPEED, (char*)&arg ) == -1 )
      {
        cerr << "Error while setting SNDCTL_DSP_SPEED to " << arg << "Hz\n";
        ::close( fd );
        return( false );
      }

    int	bufferSize = freqDsp / 40 * hdr.channels;	// 1/40 sec.
    // en exposant de 2
    unsigned	fragments = (unsigned) ::ceil( log( double(bufferSize) ) 
                                             / log( 2.0 ) + 0.5 );
    bufferSize = 1U << (fragments - 1);	// modif au plus proche
    //cout << "targetted buffer size : " << bufferSize << endl;

    // 0xMMMMSSSS, MMMM = nb de fragments (2), SSSS = taille en exposant de 2
    fragments |= 0x20000;
    if( ioctl( fd, SNDCTL_DSP_SETFRAGMENT, (char*)&fragments ) == -1 )
      {
        cerr << "Couldn't set buffers size and numbers\n";
        ::close( fd );
        return( false );
      }
    bufferSize = 1U << ( ( fragments & 0xFFFF ) - 1 );
    //cout << "actual buffer size : " << bufferSize << endl;


#else	// not linux
#if defined( __sun )
    //	Why must we ask 4 kHz to obtain 8 ????????
    int	freqDsp = hdr.rate / 2;			// 8 kHz
    int	bufferSize = 512;			// buffer de 512 octets

    audio_info_t	auinf;

    AUDIO_INITINFO( &auinf );

    auinf.output_muted = 0;			// non mute
    auinf.play.sample_rate = freqDsp;
    auinf.play.channels = hdr.channels;
    auinf.play.precision = hdr.sampleSize * 8; // * channels ??
    auinf.play.encoding = AUDIO_ENCODING_LINEAR;	// 8 bits unsigned
    auinf.play.gain = AUDIO_MAX_GAIN / 2;		// volume max
    //auinf.play.port = AUDIO_SPEAKER;		// sortie haut-parleur interne
    // is it necessary to fill this ?
    //auinf.play.avail_ports = AUDIO_SPEAKER | AUDIO_HEADPHONE | AUDIO_LINEOUT;
    auinf.play.buffer_size = bufferSize;
    auinf.play.balance = AUDIO_MID_BALANCE;

    // write new values
    if( ioctl( fd, AUDIO_SETINFO, (char*)&auinf ) == -1 )
      {
        if( errno == EINVAL )
          cerr << "EINVAL\n";
        else if( errno == EBUSY )
          cerr << "EBUSY\n";
        cerr << "Failed to set audio params.\n";
        ::close( fd );
        return( false );
      }
    /*cout << "freq : " << auinf.play.sample_rate << ", precision : " 
      << auinf.play.precision << ", encoding : " << auinf.play.encoding 
      << ", buffsize : " << auinf.play.buffer_size << endl;*/

#else	// pas sun non plus: device non reconnu
    ::close( fd );
    return( false );
#endif
#endif
    return( true );
  }
#endif	// sound compiled

}


void QAbout::music()
{
#if !defined( ABOUT_NO_SOUND ) && !defined( _WIN32 ) && defined( RR_DSP )
  RRDspSound	*dsp 
    = dynamic_cast<RRDspSound *>( &RRSoundProcessor::processor() );

  if( dsp )
    dsp->disable();

#ifdef __linux
  const char	audiodev[] = "/dev/dsp";
#else
  const char	audiodev[] = "/dev/audio";
#endif

  //	start playing sound if sound file and audio device are both OK
  struct stat	buf;
  string	name = d->musicfile;

  if( !stat( audiodev, &buf ) && !stat( name.c_str(), &buf ) )
    {
      if( d->sndFD < 0 )
	d->sndFD = open( audiodev, O_WRONLY );
      if( d->sndFD < 0 )
	return;

      ifstream		sndstr( name.c_str(), ios::in | ios::binary );
      sndstr.unsetf( ios::skipws );

      DiffCode::CompressInfo	info;
      WavHeader			& hdr = info.hdr;
      DiffCode::CompressedPos	pos;

      try
        {
          if( d->diffcoded )
            {
              info.read( sndstr, name );
              pos = DiffCode::CompressedPos( 0, hdr.channels );
            }
          else
            hdr.read( sndstr, name );
        }
      catch( exception & e )
        {
          cerr << e.what() << endl;
          return;
        }

      d->sndFD = dsp->dspFD();
      if( d->sndFD < 0 )
	return;
      dsp->stop();	// let ME use sound

      if( !initSound( d->sndFD, hdr ) )
	return;

      vector<char>	mbuf(1024 * hdr.sampleSize * hdr.channels);
      unsigned		n, sz = hdr.size;

      while( sz > 0 && d->threadRunning )
	{
          n = 1024;
          if( sz < n )
            n = sz;
          sz -= n;
          if( d->diffcoded )
            pos = DiffCode::uncompress( sndstr, info, &mbuf[0], pos, n );
          else
            sndstr.read( &mbuf[0], n );
	  //	*** Solaris: flush ***
#ifdef __sun
	  ioctl( d->sndFD, AUDIO_DRAIN, (char *)0 );
#endif	// sun
	  write( d->sndFD, &mbuf[0], n * hdr.sampleSize * hdr.channels );
	}

      sndstr.close();
    }
  d->threadRunning = false;
  return;

#endif	// ABOUT_NO_SOUND
}


void QAbout::keyPressEvent( QKeyEvent* kev )
{
  if( kev->ascii() == '+' )
    {
      ++d->edit->speed;
      kev->accept();
    }
  else if( kev->ascii() == '-' )
    {
      if( d->edit->speed > 1 )
        --d->edit->speed;
      kev->accept();
    }
  else if( kev->key() == Qt::Key_Enter || kev->key() == Qt::Key_Return )
    accept();
  else if( kev->ascii() == ' ' )
    {
      if( d->threadRunning )
	{
	  d->threadRunning = false;
#if( defined( __linux ) && !defined( ABOUT_NO_SOUND ) )
	  void	*garbage;
	  pthread_join( d->musThrd, &garbage );
          if( d->sndFD >= 0 )
            ioctl( d->sndFD, SNDCTL_DSP_RESET, (char*)0 );
#endif	// ABOUT_NO_SOUND
	}
      else
	{
	  d->threadRunning = true;
#if( !defined( _WIN32 ) && !defined( ABOUT_NO_SOUND ) )
	  pthread_create( &d->musThrd, 0, musicThread, this );
#endif
	}
    }
  else
    kev->ignore();
}

