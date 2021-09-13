
#include <roll/gui/mainWin.h>
#include <qapplication.h>
#include <qfont.h>
#include <qdir.h>
#include <string>
#include <roll/game/init.h>
#include <roll/somasound/somasoundprocessor.h>
#include "../libroll/sound/rollsoundbank.h"
#include <roll/game/vars.h>
#include <roll/socket/qPlayerServer.h>
#include "../libroll/somasound/somasoundoss.h"
#include "../libroll/somasound/somasoundalsa.h"
#include "../libroll/somasound/somaqsound.h"
#include "../libroll/somasound/somasoundqtphonon.h"
//#include "../libroll/somasound/somaqaudiooutput.h"
#include <qtranslator.h>
#ifdef __APPLE__
#include <unistd.h>
#include <fstream>
#endif

using namespace roll;
using namespace soma;
using namespace std;


int main( int argc, char** argv )
{
  /*
#ifdef __APPLE__
    char	wd[1024];
    getcwd( wd, 1024 );
    ofstream	f( "/tmp/qroll.log" );
    f << "QRoll log MacOSX\n";
    f << wd << endl;
#endif
  */

#ifdef ANDROID
    putenv( "ROLLDIR=/sdcard0/share/qroll" );
    /*
    ofstream log( "/sdcard/share/qroll/qroll-log.log" );
    log << "argv0: " << argv[0] << endl;

    char ** e = environ;
    while( *e )
    {
      log << *e << endl;
      ++e;
    }
    */
#endif

  setArgv0( argv[0] );

  if( argc > 1 && ( string( argv[1] ) == "-h"
                    || string( argv[1] ) == "--help" ) )
  {
    cout << "qroll -- Rock'N'Roll Boulderdash-like game.\n";
    exit(0);
  }

  // QApplication::setColorSpec( QApplication::ManyColor );
  new QApplication( argc, argv );
  qApp->setApplicationName( "QRoll" );
#if defined( _WIN32 ) && QT_VERSION < 0x040000
  QPixmap::setDefaultOptimization( QPixmap::MemoryOptim );
#endif

  init( argc, argv );		// initialise le jeu, charge les données
#ifndef SOMA_NO_SOUND
//  qDebug( "QAudioOutput sound" );
//  new SomaQAudioOutput;
#ifdef SOMA_SOUND_ALSA
  qDebug( "ALSA enabled." );
  new SomaSoundAlsa;
#else // alsa works better for now...
#ifdef QT_PHONON_LIB
  qDebug( "Phonon enabled." );
  new SomaSoundQtPhonon;
#endif
#endif
#ifdef SOMA_SOUND_OSS
  qDebug( "OSS sound enabled." );
  new SomaSoundOSS;
#endif
#ifndef SOMA_NO_QSOUND
  qDebug( "QSound enabled." );
  new SomaQSound;
#endif
#endif

  SomaSoundProcessor::processor().setSoundBank( new RollSoundBank );
  SomaSoundProcessor::processor().soundBank().init();
  SomaSoundProcessor::processor().soundBank().loadSounds();
  qDebug( "sounds loaded\n" );

  //	translator
  string	path = RR_path + "/po/";
  string	lang = "en";

  const char	*lgenv = getenv( "LANG" );
  if( lgenv )
    {
      string	lenv( lgenv );
      string::size_type	pos = lenv.find( '_' );
      if( pos != string::npos )
	lenv.erase( pos, lenv.length() - pos );
      QDir	dir( (path + lenv).c_str() );
      if( dir.exists() )
	lang = lenv;
    }
  QRMainWin::setLanguage( lang );

  path += lang;
  QTranslator	*tr = new QTranslator( qApp );
  tr->setObjectName( "Translator" );

  if( tr->load( "qroll", path.c_str() ) )
    qApp->installTranslator( tr );
  else
    {
      err << "warning: translation file not found\n";
      err << "path : " << path << "\nfile: qroll\n";
      delete tr;
    }

  //	static instance to fill server variable
  static QPlayerServer	qplayerserver;
  // work around an ugly bidouille
  theGame().tbct.players = PlayerServer::server;

  out << "creating main window\n" << flush;
  QRMainWin	mwin;
  out << "created.\n" << flush;

  qApp->connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );

  QString	cap = mwin.windowTitle();
  string	cap2 = cap.toUtf8().data();

  cap2 += " v. ";
  cap2 += rrVersionString();
  mwin.setWindowTitle( cap2.c_str() );
  out << "showing\n" << flush;
  mwin.show();
  out << "shown\n" << flush;

  bool	allowSound = true;

  if( argc > 1 && string( argv[1] ) == "mute" )
    allowSound = false;

  // sur sun (stations de TRAVAIL), on reste discrêt... ;-)
#if defined( sun ) || defined( __sun )
  allowSound = false;
#endif

  if( !allowSound )
    {
      out << "Disabling sound processor\n";
      mwin.soundOnOff();
    }

  out << "running loop\n" << flush;
  return( qApp->exec() );
}


