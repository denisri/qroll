
#include <roll/gui/mainWin.h>
#include <qapplication.h>
#include <qfont.h>
#include <qdir.h>
#include <string>
#include <roll/game/init.h>
#include <roll/sound/soundProcessor.h>
#include <roll/game/vars.h>
#include <roll/socket/qPlayerServer.h>
#include <roll/sound/dsp.h>
#include <roll/sound/qsound.h>
#include <qtranslator.h>
#ifdef __APPLE__
#include <unistd.h>
#include <fstream>
#endif

using namespace roll;
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

  setArgv0( argv[0] );

  // QApplication::setColorSpec( QApplication::ManyColor );
  new QApplication( argc, argv );
#ifdef _WIN32
  QPixmap::setDefaultOptimization( QPixmap::MemoryOptim );
#endif

  init( argc, argv );		// initialise le jeu, charge les donn�s
#ifdef RR_DSP
  new RRDspSound;
#endif
  new RRQSound;

  //	translator
  string	path = RR_path + "/po/";
  string	lang = "default";

  const char	*lgenv = getenv( "LANG" );
  if( lgenv )
    {
      string	lenv( lgenv );
      unsigned	pos = lenv.find( '_' );
      if( pos != string::npos )
	lenv.erase( pos, lenv.length() - pos );
      QDir	dir( (path + lenv).c_str() );
      if( dir.exists() )
	lang = lenv;
    }
  QRMainWin::setLanguage( lang );

  path += lang;
  QTranslator	*tr = new QTranslator( qApp, "Translator" );

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

  QRMainWin	mwin;

  qApp->setMainWidget( &mwin );

  QString	cap = mwin.caption();
  string	cap2 = (const char *) cap;

  cap2 += " v. ";
  cap2 += rrVersionString();
  mwin.setCaption( cap2.c_str() );
  mwin.show();

  bool	allowSound = true;

  if( argc > 1 && string( argv[1] ) == "mute" )
    allowSound = false;

  // sur sun (stations de TRAVAIL), on reste discr�... ;-)
#if defined( sun ) || defined( __sun )
  allowSound = false;
#endif

  if( !allowSound )
    {
      out << "Disabling sound processor\n";
      mwin.soundOnOff();
    }

  return( qApp->exec() );
}


