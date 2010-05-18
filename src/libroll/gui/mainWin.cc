/***************************************************************************
                          mainWin.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivi�e
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


#include <roll/gui/mainWin.h>
#include <roll/gui/clientDialog.h>
#include <roll/gui/qAbout.h>
#include <roll/socket/qPlayerServer.h>
#include <roll/game/random.h>
#include <roll/gui/configWin.h>
#include <roll/gui/editPalette.h>
#include <roll/gui/levelParams.h>
#include <roll/struct/series.h>
#include <roll/struct/seriesManager.h>
#include <roll/struct/simpleLevel.h>
#include <roll/gui/seriesArranger.h>
#include <roll/gui/painter.h>
#include <roll/gui/gameConfig.h>
#include <roll/sound/soundProcessor.h>

#include <qapplication.h>
#include <qmenubar.h>
#if QT_VERSION >= 0x040000
#include <q3popupmenu.h>
#include <q3accel.h>
#include <q3filedialog.h>
typedef Q3PopupMenu QPopupMenu;
typedef Q3Accel QAccel;
#else
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qfiledialog.h>
typedef QFileDialog Q3FileDialog;
#endif
#include <qpixmap.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qimage.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qmessagebox.h>

#include <assert.h>
#include <iostream>

#ifndef _WIN32
#include <sys/time.h>
#include <unistd.h>
#else	// _WIN32
#include <wtypes.h>
#include <winbase.h>
#include <winsock.h>
#include <time.h>
#endif

#include <roll/game/vars.h>


#include "fileopen_xpm.xpm"
#include "play_xpm.xpm"
#include "pause_xpm.xpm"
#include "stop_xpm.xpm"

using namespace roll;
using namespace std;
#if QT_VERSION >= 0x040000
using namespace Qt;
#endif


QRMainWin	*roll::theQRWin = 0;

struct QRMainWin::Private
{
  Private() 
    : timing( 0 ), config( 0 ), opengl( false ), editM( 0 ), 
      editmode( false ), editstat( 0 ), editpal( 0 ), levelparams( 0 ), 
      filedialog( 0 ) {}
  ~Private()
  {
    delete config;
    delete editpal;
    unsigned i, n = painters.size();
    for( i=0; i<n; ++i )
      delete painters[i];
  }

  Q3FileDialog *fileDialog()
  {
    if( !filedialog )
      filedialog = new Q3FileDialog( seriespath, filesfilter, theQRWin, 
				     "r&r filedialog", true );
    return( filedialog );
  }

  int			timing;
  QRConfigWin		*config;
  bool		opengl;
  QPopupMenu		*editM;
  bool		editmode;
  QLabel		*editstat;
  QREditPalette	*editpal;
  QLevelParams	*levelparams;
  QString		filesfilter;
  QString		seriespath;
  Q3FileDialog		*filedialog;
  vector<Painter*>      painters;
};


QRMainWin::QRMainWin( QWidget *parent, const char *name ) 
  : QMainWindow( parent, name ), _paused( false ), 
    _tempo( 75 ), _tick( false ), d( new Private )
{
  theQRWin = this;

  //	Sprites

  _greySprite[0] = 0;
  _greySprite[256] = 0;
  _colorSprite[0] = 0;
  _colorSprite[256] = 0;

  initPixmaps();	// init sprites (graphics)

  //    painters

  d->painters.push_back( new DrawPainter( this ) );
  d->painters.push_back( new FillPainter( this ) );
  d->painters.push_back( new RectPainter( this ) );
  d->painters.push_back( new CopyPainter( this ) );
  d->painters.push_back( new PatternPainter( this ) );

  //	signaux intra-classe

  connect( this, SIGNAL( stageChanged( unsigned ) ), 
	   SLOT( changeLevelMyself( unsigned ) ) );

  //	Interface

  QPixmap	openIcon( (const char **) fileopen_xpm );
  QPixmap	playIcon( (const char **) play_xpm );
  QPixmap	stopIcon( (const char **) stop_xpm );
  QPixmap	pauseIcon( (const char **) pause_xpm );

  _playField = new QRPlayField( this, usesOpenGL(), this, "RRPlayField" );

  setCentralWidget( _playField );
  setFocusPolicy( StrongFocus );
  setFocusProxy( _playField );
  _playField->setFocus();
  _playField->setPlayer( PlayerServer::server->newPlayer() );

  //	Menus

  _file = new QPopupMenu;
  menuBar()->insertItem( tr( "File" ), _file );
  _file->insertItem( openIcon, tr( "Open" ), this, SLOT( load() ), 
		    CTRL+Key_O, 0 );
  _file->insertSeparator();
  _file->insertItem( openIcon, tr( "Append levels..." ), this, 
		     SLOT( mergeSeries() ) );
  _file->insertItem( tr( "Save" ), this, SLOT( save() ) );
  _file->insertItem( tr( "Save as..." ), this, SLOT( saveAs() ) );
  _file->insertSeparator();
  _file->insertItem( tr( "Quit" ), qApp, SLOT(quit()), CTRL+Key_Q );

  _game = new QPopupMenu;
  menuBar()->insertItem( tr( "Game" ), _game );
  _game->insertItem( playIcon, tr( "Start game" ), this, SLOT(start()), 
		     CTRL+Key_J, 0 );
  _game->insertItem( stopIcon, tr( "Stop game" ), this, SLOT(stop()), 
		     CTRL+Key_S, 1 );
  _game->insertSeparator();
  _game->insertItem( pauseIcon, tr( "Pause" ), this, SLOT(pause()), 
		     CTRL+Key_P, 2 );
  _game->insertSeparator();
  _game->insertItem( tr( "Configure" ), this, SLOT( configure() ), 0, 3 );

  _game->setItemEnabled( 1, false );
  _game->setItemEnabled( 2, false );

  QPopupMenu	*stage = new QPopupMenu;
  menuBar()->insertItem( tr( "Level" ), stage );
  stage->insertItem( tr( "Next level" ), this, SLOT( nextStage() ), 
		     Key_Plus, 0 );
  stage->insertItem( tr( "Previous level" ), this, SLOT( prevStage() ), 
		     Key_Minus, 1 );

  _networkMen = new QPopupMenu;
  menuBar()->insertItem( tr( "Network / players" ), _networkMen );
  _networkMen->insertItem( tr( "Create server" ), this, 
			   SLOT( netServer() ), 0, 0 );
  _networkMen->insertItem( tr( "Client" ), this, SLOT( netClient() ), 0, 1 );
  _networkMen->insertItem( tr( "Close connection" ), this, 
			   SLOT( netClose() ), 0, 2 );
  _networkMen->setItemEnabled( 2, false );
  _networkMen->insertSeparator();
  _networkMen->insertItem( tr( "Add a local player" ), this, 
			   SLOT( addPlayer() ), 0, 3 );
  _networkMen->insertItem( tr( "Remove a local player" ), this, 
			   SLOT( removePlayer() ), 0, 4 );
  _networkMen->setItemEnabled( 4, false );

  _soundM = new QPopupMenu;
  menuBar()->insertItem( tr( "Sound" ), _soundM );
  _soundM->insertItem( tr( "Sound on/off" ), this, 
		       SLOT( soundOnOff() ), 0, 0 );
  if( !RRSoundProcessor::processor().isOK() )
    _soundM->setItemEnabled( 0, false );
  else
    _soundM->setItemChecked( 0, true );

  _viewM = new QPopupMenu;
  menuBar()->insertItem( tr( "View" ), _viewM );
  _viewM->insertItem( tr( "Allow zoom (very fast CPU/graphics card " 
			  "only !!)" ), 
		      this, SLOT( allowScale() ), 0, 0 );
  _viewM->insertItem( tr( "Standard size" ), this, 
		      SLOT( standardSize() ), 0, 1 );

  d->editM = new QPopupMenu;
  menuBar()->insertItem( tr( "Edit" ), d->editM );
  d->editM->insertItem( tr( "Edit level" ), this, SLOT( editLevel() ), 0, 0 );
  d->editM->insertItem( tr( "Level parameters..." ), this, 
			SLOT( levelParams() ), 0, 1 );
  d->editM->insertSeparator();
  d->editM->insertItem( tr( "Insert new level" ), this, SLOT( insertLevel() ), 
			0, 3 );
  d->editM->insertItem( tr( "Delete current level" ), this, 
			SLOT( deleteLevel() ), 0, 4 );
  d->editM->insertSeparator();
  d->editM->insertItem( tr( "Series arrangement" ), this, 
			SLOT( arrangeSeries() ), 0, 6 );
  d->editM->setItemChecked( 6, false );

  QPopupMenu	*helpM = new QPopupMenu;
  menuBar()->insertSeparator();
  menuBar()->insertItem( tr( "Help" ), helpM );
  helpM->insertItem( tr( "About..." ), this, SLOT( about() ) );


  //	Icons bar

  //cout << "icons...\n";
#if QT_VERSION >= 0x040000
  _tools = addToolBar( tr( "file operations" ) );
  _tools->setIconSize( QSize( 20, 20 ) );
#else
  _tools = new QToolBar( this, "file operations" );
#endif

  _openBtn = new QToolButton( openIcon, tr( "Open levels" ), 
			      tr( "Loads a Rock'n'roll levels file, " 
				  "replacing the current levels set" ), this, 
			      SLOT(load()), _tools, "open" );

#if QT_VERSION >= 0x040000
  QAction *a = _tools->addWidget( _openBtn );
  a->setVisible( true );
#endif
  _tools->addSeparator();

  _playBtn = new QToolButton( playIcon, tr( "Start game" ), 
			      tr( "runs a new game" ),
			      this, SLOT(start()), _tools, "play" );
  _stopBtn = new QToolButton( stopIcon, tr( "Stop game" ), 
			      tr( "Finishes the current game" ),
			      this, SLOT(stop()), _tools, "stop" );
  _pauseBtn = new QToolButton( pauseIcon, tr( "Pause" ), 
			       tr( "Holds game or return to it" ), this, 
			       SLOT(pause()), _tools, "pause" );

#if QT_VERSION >= 0x040000
  a = _tools->addWidget( _playBtn );
  a->setVisible( true );
  a = _tools->addWidget( _stopBtn );
  a->setVisible( true );
  a = _tools->addWidget( _pauseBtn );
  a->setVisible( true );
#endif

  _stopBtn->setEnabled( false );
  _pauseBtn->setEnabled( false );

  /*
  _tools->addSeparator();

  QToolButton *b = QWhatsThis::whatsThisButton( _tools );
#if QT_VERSION >= 0x040000
  a = _tools->addWidget( b );
  a->setVisible( true );
#endif
  QWhatsThis::add( _openBtn, tr( "Loads levels - really simple !" ) );
  QWhatsThis::add( _playBtn, 
		   tr( "Play game - alone or with a network" ) );
  QWhatsThis::add( _stopBtn,	
		   tr( "well, this just STOPS the game. Clear ?" ) );
  QWhatsThis::add( _pauseBtn, tr( "Ooh, can't remember what's this for..." ) );
  */

  //	Status bar setup
  d->editstat = new QLabel( "  ", statusBar() );
  d->editstat->setPalette( QPalette( QColor( 255, 192, 192 ) ) );
  statusBar()->addWidget( d->editstat, 0, true );
  statusBar()->message( tr( "Ready" ) );

#if QT_VERSION >= 300
  resize( _playField->width(), menuBar()->sizeHint().height() 
  	  + _tools->sizeHint().height() 
	  + 394 + statusBar()->sizeHint().height() );
#else
  resize( _playField->width(), menuBar()->height() + _tools->height() + 
	  _playField->height() + statusBar()->height() );
#endif

  QPixmap	rpix( (RR_path + "/icons/rockroll.xpm").c_str() );
  if( rpix.isNull() )
    cerr << "Pas d'ic�e\n";
  else setIcon( rpix );
  setCaption( tr( "QRoll, heir of Rock'N'Roll" ) );

  load( (RR_path + "/levels/serie0.r&r").c_str() );

  //	Timer

  _timer = new QTimer( this, "tempo" );

  connect( _timer, SIGNAL( timeout() ), this, SLOT( timeStep() ) );
  _timer->start( _tempo );

  _clockTimer = new QTimer( this, "clock" );
  connect( _clockTimer, SIGNAL( timeout() ), this, SLOT( clockTick() ) );

  d->filesfilter = tr( "Rock'n'roll series" ) + "( *.r&r );;" + 
    tr( "All files" ) + "( * )";
  d->seriespath = ( RR_path + "/levels" ).c_str();

  // load game and GUI config

  GameConfig::config().load();
}


QRMainWin::~QRMainWin()
{
  unsigned			i;
  set<QRPlayField *>::iterator	iw;

  //delete _socket;
  for( iw=_otherWins.begin(); iw!=_otherWins.end(); ++iw )
    (*iw)->close( true );

  //	sprites

  for( i=0; i<240; ++i )
    {
      delete _sprite[i];
      delete _sprite[ i + 256 ];
      delete _osprite[i];
      delete _osprite[ i + 256 ];
    }
  delete _sprite[255];
  delete _osprite[255];

  delete _greySprite[0];
  delete _greySprite[256];
  delete _colorSprite[0];
  delete _colorSprite[256];

  delete d;
}


void QRMainWin::load()
{
  Q3FileDialog	*fd = d->fileDialog();
  fd->setFilters( d->filesfilter );
  fd->setMode( Q3FileDialog::ExistingFile );
  fd->setCaption( tr( "Load Rock'n'Roll levels" ) );
  if( fd->exec() )
    {
      QString fn = fd->selectedFile();
      if ( !fn.isEmpty() )
	{
	  load( fn );
	  return;
	}
    }
  statusBar()->message( tr( "Loading aborted" ), 2000 );
}


void QRMainWin::mergeSeries()
{
  Q3FileDialog	*fd = d->fileDialog();
  fd->setFilters( d->filesfilter );
  fd->setMode( Q3FileDialog::ExistingFile );
  fd->setCaption( tr( "Append Rock'n'Roll levels" ) );
  if( fd->exec() )
    {
      QString fn = fd->selectedFile();
      if ( !fn.isEmpty() )
	{
	  if( ser->load( fn.ascii() ) )
	    {
	      statusBar()->message( tr( "Merge succeeded." ), 2000 );
	      emit seriesChanged();
	    }
	  else
	    statusBar()->message( tr( "Merge failed !" ), 2000 );
	  return;
	}
    }
  statusBar()->message( tr( "Loading aborted" ), 2000 );
}


void QRMainWin::load( const char* filename )
{
  out << "load " << filename << "\n";
  if( SeriesManager::load( filename ) )
    {
      game.tbct.setModified( false );
      emit stageChanged( 0 );
      emit seriesChanged();
    }
  else
    statusBar()->message( tr(  "Couldn't load series " ) + filename + 
			  tr( " - bad file ?" ), 5000 );
}


void QRMainWin::save()
{
  if( ser->filename().empty() )
    {
      saveAs();
      return;
    }
  save( ser->filename().c_str() );
}


void QRMainWin::saveAs()
{
  Q3FileDialog	*fd = d->fileDialog();
  fd->setFilters( d->filesfilter );
  fd->setMode( Q3FileDialog::AnyFile );
  fd->setCaption( tr( "Save Rock'n'Roll levels" ) );
  QString	startf = ser->filename().c_str();
  if( !startf.isEmpty() )
    fd->setDir( startf );
  if( fd->exec() )
    {
      QString fn = fd->selectedFile();
      if ( !fn.isEmpty() )
	{
	  save( fn );
	  return;
	}
    }
  statusBar()->message( tr( "Saving aborted" ), 2000 );
}


void QRMainWin::save( const char* filename )
{
  storeLevel();
  if( ser->save( filename ) )
    statusBar()->message( "Save succeeded.", 2000 );
  else
    statusBar()->message( "Save FAILED ! (unable to write in specified "
			  "directory ?)", 5000 );
}


void QRMainWin::start()
{
  changeLevelMyself( game.tb );
  PlayerServer	& ps = *PlayerServer::server;
  ps.flushKbd();

  if( ps.mode() == PlayerServer::Server )
    {
      unsigned	rseed = time( 0 );
      out << "random seed : " << rseed << endl;
      PlayerServer::server->startGame( game.tb, rseed, "unknown_series" );
      rrand.srand( rseed );
    }

  if( d->config )
    delete d->config;	// close config dialog during game

  game.run( game.tb );
  game.init( game.tb );

  _clockTimer->start( 1000 );

  statusBar()->message( tr( "Running" ) );
  _paused = false;

  _file->setItemEnabled( 0, false );
  _game->setItemEnabled( 0, false );
  if( ps.mode() == PlayerServer::Client )
    _game->setItemEnabled( 1, false );
  else
    _game->setItemEnabled( 1, true );
  _game->setItemEnabled( 2, true );
  _game->setItemChecked( 2, false );
  _game->setItemEnabled( 3, false );	// configure
  setEditMode( false );
  d->editM->setItemEnabled( 0, false );
  d->editM->setItemEnabled( 3, false );
  d->editM->setItemEnabled( 4, false );
  d->editM->setItemEnabled( 6, false );

  _openBtn->setEnabled( false );
  _playBtn->setEnabled( false );
  if( ps.mode() == PlayerServer::Client )
    _stopBtn->setEnabled( false );
  else
    _stopBtn->setEnabled( true );
  _pauseBtn->setEnabled( true );
}


void QRMainWin::stop()
{
  _clockTimer->stop();
  emit timeTick();
  game.running = false;
  game.paused = false;

  PlayerServer	& ps = *PlayerServer::server;

  ps.stopGame();
  ps.flushKbd();
  game.init( game.tb );
  emit gameStopped();

  statusBar()->message( tr( "Ready" ) );
  _paused = false;

  _file->setItemEnabled( 0, true );	// open
  if( ps.mode() == PlayerServer::Client )	// play
    _game->setItemEnabled( 0, false );
  else
    _game->setItemEnabled( 0, true );
  _game->setItemEnabled( 1, false );	// stop
  _game->setItemEnabled( 2, false );	// pause
  _game->setItemChecked( 2, false );
  _game->setItemEnabled( 3, true );	// config
  d->editM->setItemEnabled( 0, true );
  d->editM->setItemEnabled( 3, true );
  d->editM->setItemEnabled( 4, true );
  d->editM->setItemEnabled( 6, true );

  if( ps.mode() == PlayerServer::Client )	// open
    _openBtn->setEnabled( false );
  else
    _openBtn->setEnabled( true );
  if( ps.mode() == PlayerServer::Client )	// play
    _playBtn->setEnabled( false );
  else
    _playBtn->setEnabled( true );
  _stopBtn->setEnabled( false );
  _pauseBtn->setEnabled( false );
}


void QRMainWin::pause()
{
  PlayerServer::server->pauseGame( !_paused );
  if( _paused )
    pauseOFF();
  else
    pauseON();
}


void QRMainWin::gamePauses( bool state )
{
  if( state )
    pauseON();
  else
    pauseOFF();
}


void QRMainWin::pauseOFF()
{
  game.paused = false;
  PlayerServer::server->flushKbd();
  statusBar()->message( tr( "Running" ) );
  _paused = false;
  _clockTimer->start( 1000 );

  if( _game->isItemChecked( 2 ) )
    _game->setItemChecked( 2, false );
  else out << "Menu Pause mal check�n";
}


void QRMainWin::pauseON()
{
  _clockTimer->stop();
  game.paused = true;
  PlayerServer::server->flushKbd();
  statusBar()->message( tr( "Paused" ) );
  _paused = true;

  if( _game->isItemChecked( 2 ) )
    out << "Menu Pause mal check�n";
  else _game->setItemChecked( 2, true );
}


void QRMainWin::nextStage()
{
  if( !game.running && game.tb < ((int) ser->numLevels()) - 1 )
    emit stageChanged( game.tb + 1 );
}


void QRMainWin::prevStage()
{
  if( !game.running && game.tb > 0 )
    emit stageChanged( game.tb - 1 );
}


void QRMainWin::keyPressEvent( QKeyEvent* key )
{
  // cout << "keypress: " << key->key() << endl;
  _playField->keyPressedEvent( key );
}


void QRMainWin::keyReleaseEvent( QKeyEvent* key )
{
  /*
  cout << "keyrelease: " << key->key() << endl;
  cout << "ascii: " << key->ascii() << endl;
  cout << "state: " << key->state() << endl;
  cout << "stateAfter: " << key->stateAfter() << endl;
  cout << "text: " << key->text() << endl;
  cout << "count: " << key->count() << endl;
  */
  _playField->keyReleasedEvent( key );
}


void QRMainWin::netServer()
{
  statusBar()->message( tr( "Connecting to network..." ) );
  _networkMen->setItemEnabled( 0, false );
  _networkMen->setItemEnabled( 1, false );
  _networkMen->setItemEnabled( 2, true );

  QPlayerServer	*qp = dynamic_cast<QPlayerServer *>( PlayerServer::server );
  if( qp )
    {
      connect( qp, SIGNAL( networkConnected() ), this, 
	       SLOT( localServerConnected() ) );
      connect( qp, SIGNAL( netClientConnected( int ) ), this, 
	       SLOT( remoteClientConnected( int ) ) );
      connect( qp, SIGNAL( netClientDisconnected( int ) ), this, 
	       SLOT( remoteClientDisconnected( int ) ) );
      connect( qp, SIGNAL( netError( int ) ), this, SLOT( netError( int ) ) );
      connect( qp, SIGNAL( gamePaused( bool ) ), this, 
	       SLOT( gamePauses( bool ) ) );
      connect( qp, SIGNAL( turnEndEmitted() ), this, 
	       SLOT( turnEndEmitted() ) );
    }
  else
    cerr << "BUG: PlayerServer is not a QPlayerServer\n";

  PlayerServer::server->makeServer( 2000 );
}


void QRMainWin::netClient()
{
  RRClientDialog	cd( this );
  int res = cd.exec();
  if( res == QDialog::Accepted )
    {
      statusBar()->message( tr( "Connecting to network..." ) );
      _networkMen->setItemEnabled( 0, false );
      _networkMen->setItemEnabled( 1, false );
      _networkMen->setItemEnabled( 2, true );
      _file->setItemEnabled( 0, false );	// open
      _game->setItemEnabled( 0, false );	// play
      _game->setItemEnabled( 1, false );	// stop
      _playBtn->setEnabled( false );
      _stopBtn->setEnabled( false );
      _openBtn->setEnabled( false );

      d->timing = 0;

      QPlayerServer	*qp 
	= dynamic_cast<QPlayerServer *>( PlayerServer::server );
      if( qp )
	{
	  connect( qp, SIGNAL( netHostFound() ), this, 
		   SLOT( clientHostFound() ) );
	  connect( qp, SIGNAL( networkConnected() ), this, 
		   SLOT( localClientConnected() ) );
	  connect( qp, SIGNAL( netError( int ) ), this, 
		   SLOT( netError( int ) ) );
	  connect( qp, SIGNAL( connectionClosed() ), this, 
		   SLOT( remoteServerDisconnected() ) );
	  connect( qp, 
		   SIGNAL( gameStarted( unsigned, unsigned, 
					const std::string & ) ),
		   this, 
		   SLOT( gameStarts( unsigned, unsigned, 
				     const std::string & ) ) );
	  connect( qp, SIGNAL( gamePaused( bool ) ), this, 
		   SLOT( gamePauses( bool ) ) );
	  connect( qp, SIGNAL( gameStopped() ), this, SLOT( stop() ) );
	  connect( qp, SIGNAL( turnEnded() ), this, SLOT( turnEnds() ) );
	  connect( qp, SIGNAL( playersRenumed( const roll::RenumList & ) ), 
		   this, SLOT( playersRenumed( const roll::RenumList & ) ) );
	}
      else
	cerr << "BUG: PlayerServer is not a QPlayerServer\n";

      PlayerServer::server->makeClient( cd.address(), 2000 );
    }
  else
    statusBar()->message( tr( "Connection canceled" ), 2000 );
}


void QRMainWin::netClose()
{
  PlayerServer::server->makeLocal();
  QPlayerServer	*qp = dynamic_cast<QPlayerServer *>( PlayerServer::server );
  if( qp )
    {
      qp->disconnect();
    }

  _networkMen->setItemEnabled( 0, true );
  _networkMen->setItemEnabled( 1, true );
  _networkMen->setItemEnabled( 2, false );
  statusBar()->message( tr( "Connection(s) closed." ), 2000 );
  if( game.running )
    {
      _file->setItemEnabled( 0, false );	// open
      _game->setItemEnabled( 0, false );	// play
      _game->setItemEnabled( 1, true );		// stop
      _game->setItemEnabled( 2, true );		// pause
      _playBtn->setEnabled( false );
      _stopBtn->setEnabled( true );
      _pauseBtn->setEnabled( true );
      _openBtn->setEnabled( false );
    }
  else
    {
      _file->setItemEnabled( 0, true );		// open
      _game->setItemEnabled( 0, true );		// play
      _game->setItemEnabled( 1, false );	// stop
      _game->setItemEnabled( 2, false );	// pause
      _game->setItemChecked( 2, false );
      _playBtn->setEnabled( true );
      _stopBtn->setEnabled( false );
      _pauseBtn->setEnabled( false );
      _openBtn->setEnabled( true );
    }
}


void QRMainWin::clientHostFound()
{
  statusBar()->message( tr( "Server found - connecting..." ) );
}


void QRMainWin::localClientConnected()
{
  statusBar()->message( tr( "Network connected !" ), 2000 );
  _playBtn->setEnabled( false );
}


void QRMainWin::localServerConnected()
{
  statusBar()->message( tr( "Network OK - waiting for incoming connections" ), 
			3000 );
}


void QRMainWin::remoteClientConnected( int num )
{
  statusBar()->message( tr( "Incoming connection : compuer " ) 
			+ QString::number( num ) 
			+ tr( " joins the game !" ), 3000 );
}


void QRMainWin::remoteClientDisconnected( int num )
{
  statusBar()->message( tr( "Client " ) + QString::number( num ) 
			+ tr( " leaves the game !" ), 3000 );
}


void QRMainWin::remoteServerDisconnected()
{
  statusBar()->message( tr( "Server leaved the game ! - closing." ), 3000 );
  netClose();
}


void QRMainWin::netError( int err )
{
  out << "Network error " << err << " - closing" << endl;
  netClose();
}


void QRMainWin::storeLevel()
{
  if( game.tbct.modified() )
    {
      // ask confirmation ?
      ser->pack( game.tbct, game.tb );
    }
}


void QRMainWin::changeLevelMyself( unsigned n )
{
  storeLevel();
  game.tb = n;
  game.init( game.tb );
  genPixmaps();
}


void QRMainWin::receiveKeyPressed( const QRPlayField::KeyCode & key, 
				   const QRPlayField* )
{
  PlayerServer::server->keyPressed( key.player, key.key );
}


void QRMainWin::receiveKeyReleased( const QRPlayField::KeyCode & key, 
				    const QRPlayField* )
{
  PlayerServer::server->keyReleased( key.player, key.key );
}


void QRMainWin::initPixmaps()
{
  QImage	image1( (RR_path + "/data/rockrol3.png").c_str() );
  assert( !image1.isNull() );
  QImage	image2( (RR_path + "/data/rr2_sp2b.png").c_str() );
  assert( !image2.isNull() );

  /*out << "image1 sz: " << image1.width() << " x " << image1.height() << endl;
  out << "image2 sz: " << image2.width() << " x " << image2.height() << endl;
  */

  unsigned char		*gbuf1 = new unsigned char[240*32*32], 
    *gbuf2 = new unsigned char[240*32*32];
  unsigned char		*cbuf1 = new unsigned char[240*32*32], 
    *cbuf2 = new unsigned char[240*32*32];
  unsigned		i, j;
  unsigned const	W=32, H=32, SZ=W*H;	// taille des sprites

  //	Allocation des donn�s des sprites
  for( i=0; i<240; ++i )
    {
      _greySprite[i] = gbuf1 + i*SZ;
      _greySprite[i+256] = gbuf2 + i*SZ;
      _colorSprite[i] = cbuf1 + i*SZ;
      _colorSprite[i+256] = cbuf2 + i*SZ;
    }
  for( i=241; i<256; ++i )
    {
      _greySprite[i] = 0;
      _greySprite[i+256] = 0;
      _colorSprite[i] = 0;
      _colorSprite[i+256] = 0;
    }

  //	Allocation des Pixmap

  for( i=0; i<240; ++i )
    {
      _sprite[ i     ] = new QPixmap( W, H );
      _sprite[ 256+i ] = new QPixmap( W, H );
#if !defined( _WIN32 ) || !defined( WIN32_AVOID_PIXMAPS )
      _osprite[ i     ] = new QPixmap( W, H );
      _osprite[ 256+i ] = new QPixmap( W, H );
#endif
    }

#if defined( _WIN32 ) && defined( WIN32_AVOID_PIXMAPS )
  _sprite[ 255 ] = new QPixmap( W, H );
  _sprite[ 255 ]->fill( QColor( 0, 0, 0 ) );	// vide
  _osprite[ 255 ] = new QImage( W, H, 32 );
  _osprite[ 255 ]->fill( qRgb( 0, 0, 0 ) );
#else
  _sprite[ 255 ] = new QPixmap( W, H );
  _sprite[ 255 ]->fill( QColor( 0, 0, 0 ) );	// vide
  _osprite[ 255 ] = new QPixmap( W, H );
  _osprite[ 255 ]->fill( QColor( 0, 0, 0 ) );
#endif

  for( i=240; i<255; ++i )
    {
      _sprite[ i     ] = 0;	// pr�aution anti-bug
      _sprite[ i+256 ] = 0;
      _osprite[ i     ] = 0;
      _osprite[ i+256 ] = 0;
    }

  unsigned	num;

  for( j=0; j<12; ++j )
    for( i=0; i<20; ++i )
      {
	if( j < 6 ) num = j*40 + i;
	else num = (j-6)*40 + 20 + i;
#if defined( _WIN32 ) && defined( WIN32_AVOID_PIXMAPS )
	_osprite[ num ] = new QImage( image1.copy( i*W, j*H, W, H ) );
	_osprite[ num+256 ] = new QImage( image2.copy( i*W, j*H, W, H ) );
#else
	_osprite[ num ]->convertFromImage( image1.copy( i*W, j*H, W, H ) );
	_osprite[ num+256 ]->convertFromImage( image2.copy( i*W, j*H, W, H ) );
#endif
	convertSprite( image1, i*W, j*H, W, H, 
		       _greySprite[ num ], _colorSprite[ num ] );
	convertSprite( image2, i*W, j*H, W, H, 
		       _greySprite[ num+256 ], _colorSprite[ num+256 ] );
      }
}


void QRMainWin::convertSprite( const QImage & img, unsigned x, unsigned y, 
			       unsigned w, unsigned h, 
			       unsigned char* gbuf, unsigned char* cbuf )
{
  unsigned 	i, j;
  QRgb		col;

  for( j=0; j<h; ++j )
    for( i=0; i<w; ++i )
      {
	col = img.pixel( x+i, y+j );
	/* *gbuf++ = (unsigned char) ( 0.579 * // ( ~ 1/sqrt(3) )
				    sqrt( qRed( col ) * qRed( col ) + 
					  qGreen( col ) * qGreen( col ) + 
					  qBlue( col ) * qBlue( col ) ) ); */
	//*gbuf++ = ( qRed( col ) + qGreen( col ) + qBlue( col ) ) / 3;
	*gbuf++ = max( max( qRed( col ), qGreen( col ) ), qBlue( col ) );
	*cbuf++ = (unsigned char) gameColor( col );
      }
}


void QRMainWin::genPixmaps()
{
  unsigned	i, j, num;

  QImage	img( 32, 32, 32 );

  for( j=0; j<12; ++j )
    for( i=0; i<20; ++i )
      {
	if( j < 6 ) num = j*40 + i;
	else num = (j-6)*40 + 20 + i;
	fillImage( img, num );
	_sprite[ num ]->convertFromImage( img );
	fillImage( img, num+256 );
	_sprite[ num+256 ]->convertFromImage( img );
      }
}


void QRMainWin::fillImage( QImage & img, unsigned num )
{
  unsigned		col;
  unsigned		i, j, w=img.width(), h=img.height();
  unsigned char		*bufg = _greySprite[ num ];
  unsigned char		*bufc = _colorSprite[ num ];
  WorkLevel		& tc = game.tbct;
  WorkLevel::RGB	modif;
  short			tmp1, tmp2, tmp3;

  for( j=0; j<h; ++j )
    for( i=0; i<w; ++i )
      {
	col = *bufg++;
	modif = tc.color( (WorkLevel::PartColor) *bufc++ );
	tmp1 = col + modif.r;
	if( tmp1 < 0 ) tmp1 = 0;
	if( tmp1 > 255 ) tmp1 = 255;
	tmp2 = col + modif.g;
	if( tmp2 < 0 ) tmp2 = 0;
	if( tmp2 > 255 ) tmp2 = 255;
	tmp3 = col + modif.b;
	if( tmp3 < 0 ) tmp3 = 0;
	if( tmp3 > 255 ) tmp3 = 255;
	img.setPixel( i, j, qRgb( tmp1, tmp2, tmp3 ) );
	/*if( *(bufc-1) == WorkLevel::GRASS ||
	    *(bufc-1) == WorkLevel::ROLL )
	  {
	    cout << *(bufc-1) << " : " << tmp1 << ", " << tmp2 << ", " 
		 << tmp3 << endl;
		 }*/
      }
}


WorkLevel::PartColor QRMainWin::gameColor( QRgb rgb )
{
  unsigned char r = qRed( rgb ), g = qGreen( rgb ), b = qBlue( rgb );
  //	gris ?
  if( abs( (int) r - g ) <= 5 && abs( (int) r - b ) <= 5 )
    {
      //	noir ou blanc ?
      if( r==0 || r==255 )
	return( WorkLevel::UNCHANGED );
      else	// vraiment gris
      return( WorkLevel::ROCK );
    }
  //	rouge ?
  if( r > g && r > b )
    return( WorkLevel::MONSTER );
  //	vert ?
  if( g > r && g > b )
    return( WorkLevel::GRASS );
  //	bleu ?
  if( b > r && b > g )
    return( WorkLevel::ROLL );
  //	violet ?
  return( WorkLevel::DIAM );
}


void QRMainWin::stepFull()
{
  emit displayFull();
}


void QRMainWin::stepHalf()
{
  PlayerServer	& ps = *PlayerServer::server;
  /*if( ps.mode() == QPlayerServer::Client && game.running )
    return;	// in client mode, synchro is owned by the server
  */

  if( game.running && ps.mode() != QPlayerServer::Server )
    {
      if( game.paused )
	ps.endTurn();
      else
	ps.takeKeys();
    }

  game.step();

  /*if( game.timeChanged )
    {
      // changer l'affichage du temps
      }*/

  if( game.changed )
    emit stageChanged( game.tb );
  else if( game.running )
    {
      emit displayHalf();
      emit scoreChanged();
    }
  /*if( wasRunning && !game.running )
    emit stopped();*/
}


void QRMainWin::timeStep()
{
  PlayerServer	& ps = *PlayerServer::server;
  if( game.running && ps.mode() == PlayerServer::Client )
    return;	// client timing is handled by server messages

  if( _tick )	//	1/2 timing: pour le scoll entier
    {
      _tick = false;
      stepFull();
    }
  else
    {
      if( game.running && ps.mode() == PlayerServer::Server )
	{
	  if( game.paused )
	    ps.endTurn();
	  else
	    ps.takeKeys();
	}
      else
	{
	  _tick = true;
	  stepHalf();
	}
    }
}


void QRMainWin::turnEndEmitted()
{
  _tick = true;
  stepHalf();
  // should adjust timing to get more regular updates like in client mode
  _timer->start( _tempo );
}


void QRMainWin::timeOut()
{
  game.tbct.frozen = true;
}


void QRMainWin::gameStarts( unsigned level, unsigned randSeed, 
			    const string & series )
{
  out << tr( "Starting game" ).latin1() << " ,\n"
      << tr( "Level  : " ).latin1() << level << "\n"
      << tr( "Series : " ).latin1() << series << "\n"
      << tr( "Random seed : " ).latin1() << randSeed << endl;
    //       << tr( "Nb of players : " ) << (unsigned) gs.numPlayers << "\n" 

  if( game.running )
    stop();
  game.tb = level;
  rrand.srand( randSeed );
  start();
}


void QRMainWin::addPlayer()
{
  set<unsigned>	pl = _playField->players();
  unsigned	player = PlayerServer::server->newPlayer();
  set<QRPlayField *>::const_iterator	iw, fw=_otherWins.end();

  pl.insert( player );
  _playField->setPlayers( pl );

  QRPlayField	*pf = new QRPlayField( this, usesOpenGL(), 0, "Player bis" );
  _otherWins.insert( pf );
  for( iw=_otherWins.begin(); iw!=fw; ++iw )
    (*iw)->setPlayers( pl );
  pf->setPlayer( player );
  _networkMen->setItemEnabled( 4, true );
  pf->show();
}


void QRMainWin::removePlayer()
{
  if( _otherWins.size() > 0 )
    {
      (*_otherWins.rbegin())->close( true );
    }
}


void QRMainWin::closePlayField( QRPlayField* win )
{
  set<QRPlayField *>::iterator	i = _otherWins.find( win );

  if( *i == win )
    {
      PlayerServer::server->removePlayer( win->player() );
      _otherWins.erase( i );
      if( _otherWins.size() == 0 )
	_networkMen->setItemEnabled( 4, false );
    }
}


void QRMainWin::playerRemoved( unsigned num )
{
  out << "le joueur " << num << " quitte le jeu\n";
  emit removePlayer( num );
}


void QRMainWin::clockTick()
{
  unsigned t = game.tbct.time();

  if( t == 0 )
    {
      out << "Bon, l�normalement, le temps est fini.\n";
      _clockTimer->stop();
      //emit timeout();
      return;
    }

  if( t <= 19 && t > 2 && (t & 1) )
    RRSoundProcessor::processor().process( (RollSoundBank::SNDLIST) 
    (RollSoundBank::DONG1 + 9 - t/2) );

  game.tbct.setTime( game.tbct.time() - 1 );

  emit timeTick();
}


void QRMainWin::playerAdded( unsigned num )
{
  out << "le joueur " << num+1 << " entre en jeu.\n";
}


void QRMainWin::playersRenumed( const RenumList & play )
{
  //out << "QRMainWin::playersRenumed\n";
  emit renumPlayers( play );
}


void QRMainWin::about()
{
  QAbout	ab;

  ab.exec();
}


void QRMainWin::soundOnOff()
{
  if( _soundM->isItemChecked( 0 ) )
    {
      RRSoundProcessor::processor().disable();
      _soundM->setItemChecked( 0, false );
    }
  else
    {
      RRSoundProcessor::processor().enable();
      if( RRSoundProcessor::processor().isOK() )
	_soundM->setItemChecked( 0, true );
      else
	_soundM->setItemEnabled( 0, false );
    }
}


void QRMainWin::allowScale()
{
  bool	f;

  if( _playField->scalingEnabled() )
    {
#if QT_VERSION >= 0x040000
      _viewM->changeItem( 0, tr( "Allow zoom" ) );
#else
      _viewM->changeItem( tr( "Allow zoom" ), 0 );
#endif
      statusBar()->message( tr( "was it too slow ... ?    ;-)" ), 2000 );
      f = false;
    }
  else
    {
#if QT_VERSION >= 0x040000
      _viewM->changeItem( 0, tr( "Disable zoom" ) );
#else
      _viewM->changeItem( tr( "Disable zoom" ), 0 );
#endif
      f = true;
    }

  _playField->enableScaling( f );
  set<QRPlayField *>::const_iterator	iw, fw=_otherWins.end();
  for( iw=_otherWins.begin(); iw!=fw; ++iw )
    (*iw)->enableScaling( f );
}


void QRMainWin::standardSize()
{
  //_playField->setGameSize( 512, 384 );
  resize( 640, menuBar()->frameGeometry().height() 
	  + _tools->frameGeometry().height() 
	  + statusBar()->frameGeometry().height() + 385 );

  set<QRPlayField *>::const_iterator	iw, fw=_otherWins.end();
  for( iw=_otherWins.begin(); iw!=fw; ++iw )
    (*iw)->setGameSize( 512, 384 );
}


void QRMainWin::turnEnds()
{
  /*static unsigned count = 0;
    cout << "turnEnds " << count++ << "\n";*/
      int		intv = 75;
#ifndef _WIN32
  struct timeval	tv;
  struct timezone	tz;
  gettimeofday( &tv, &tz );
  int			timing = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  if( d->timing == 0 )
    intv = 75;
  else
    intv = ( timing - d->timing ) / 2;
  d->timing = timing;
#else	// _WIN32
  LARGE_INTEGER		lpfreq;
  if( QueryPerformanceFrequency( &lpfreq ) )
    {
      LARGE_INTEGER	timer;
      int		timing;
      QueryPerformanceCounter( &timer );
      timing = int( double( timer.LowPart ) * 1000 / lpfreq.LowPart );
      if( d->timing == 0 )
	intv = 75;
      else
	intv = ( timing - d->timing ) / 2;
      d->timing = timing;
    }
  else	// low-res timer (55ms interval on Win9x: NOT enough precision !)
    {
      unsigned		timing = GetTickCount();
      if( d->timing == 0 )
	intv = 75;
      else
	intv = ( timing - d->timing ) / 2;
      d->timing = timing;
    }
#endif
  //cout << "previous turn timing : " << timing - d->timing << " ms " << endl;
  QTimer::singleShot( intv, this, SLOT( stepFull() ) );
  stepHalf();
}


void QRMainWin::configure()
{
  if( !d->config )
    {
      d->config = new QRConfigWin;
      connect( d->config, SIGNAL( closed() ), this, SLOT( configClosed() ) );
      d->config->show();
    }
}


void QRMainWin::configClosed()
{
  d->config = 0;
}


bool QRMainWin::usesOpenGL() const
{
  return( d->opengl );
}


void QRMainWin::setUseOpenGL( bool x )
{
  if( d->opengl != x )
    {
      d->opengl = x;
      _playField->setUseOpenGL( x );

      set<QRPlayField *>::iterator	iw, ew = _otherWins.end();
      for( iw=_otherWins.begin(); iw!=ew; ++iw )
	(*iw)->setUseOpenGL( x );
    }
}


void QRMainWin::setTempo( unsigned x )
{
  _tempo = x;
  _timer->start( _tempo );
}


bool QRMainWin::editMode() const
{
  return( d->editmode );
}


void QRMainWin::editLevel()
{
  bool	x = !d->editmode;
  d->editmode = x;
  d->editM->setItemChecked( 0, x );
  if( x )
    {
      d->editstat->setText( tr( " EDIT mode " ) );
      d->editpal = new QREditPalette;
      d->editpal->show();
      editModeChanged( d->editpal->drawingMode() );
      connect( d->editpal, SIGNAL( destroyed() ), this, 
	       SLOT( editPaletteClosed() ) );
      connect( d->editpal, SIGNAL( modeChanged( unsigned ) ), this, 
	       SLOT( editModeChanged( unsigned ) ) );
    }
  else
    {
      d->editstat->setText( "  " );
      delete d->editpal;
      setCursor( ArrowCursor ); // to override a bug in Qt: unsetCursor doesn't
      unsetCursor();            // work (apparently) ...
    }
}


void QRMainWin::editModeChanged( unsigned mode )
{
  QCursor curs;
  switch( mode )
    {
    case 0: // paint
      curs.setShape( CrossCursor );
      break;
    case 1: // fill
      curs.setShape( PointingHandCursor );
      break;
    case 2: // rect
      curs.setShape( SizeAllCursor );
      break;
    default:
      curs.setShape( ArrowCursor );
    }
  setCursor( curs );
}


void QRMainWin::setEditMode( bool x )
{
  if( d->editmode != x )
    editLevel();
}


void QRMainWin::editPaletteClosed()
{
  d->editpal = 0;
  setEditMode( false );
}


QREditPalette* QRMainWin::editPalette()
{
  return( d->editpal );
}


void QRMainWin::levelParams()
{
  if( !d->levelparams )
    {
      d->levelparams = new QLevelParams;
      connect( d->levelparams, SIGNAL( destroyed() ), this, 
	       SLOT( levelParamsClosed() ) );
      connect( this, SIGNAL( stageChanged( unsigned ) ), d->levelparams, 
	       SLOT( update( unsigned ) ) );
      connect( d->levelparams, SIGNAL( levelChanged( unsigned ) ), this, 
	       SIGNAL( stageChanged( unsigned ) ) );
    }
  d->levelparams->show();
}


void QRMainWin::levelParamsClosed()
{
  d->levelparams = 0;
}


void QRMainWin::insertLevel()
{
  int	res = QMessageBox::information
    ( this, tr( "Insert new level" ), 
      tr( "Insert new level before or after current one ?" ), tr( "Before" ), 
      tr( "After" ), tr( "Cancel" ) );
  int	pos = -1;
  switch( res )
    {
    case 0:	// before current
      pos = game.tb;
      break;
    case 1:	// after current
      pos = game.tb + 1;
      break;
    default:
      return;
    }
  SimpleLevel	sl( 32, 24, false, false );
  sl.makeEmpty();
  storeLevel();
  pos = ser->insertLevel( pos, sl );
  if( pos >= 0 )
    emit stageChanged( pos );
}


void QRMainWin::deleteLevel()
{
  int	res 
    = QMessageBox::warning( this, tr( "Delete level" ), 
			    tr( "Delete current level - are you sure ?" ), 
			    tr( "Yes" ), tr( "No" ) );
  if( res == 0 )
    {
      int	pos = game.tb;
      if( game.tb == (int) ser->numLevels() - 1 )
	{
	  if( ser->numLevels() == 1 )
	    {
	      QMessageBox::warning( this, tr( "Failure" ), 
				    tr( "Must keep at least one level..." ), 
				    tr( "Cancel" ) );
	      return;
	    }
	  --pos;
	}
      game.tbct.setModified( false );
      ser->deleteLevel( game.tb );
      emit stageChanged( pos );
    }
}


void QRMainWin::arrangeSeries()
{
  if( SeriesArranger::instance() )
    delete SeriesArranger::instance();
  else
    {
      SeriesArranger	*sa = new SeriesArranger;
      sa->show();
      connect( sa, SIGNAL( destroyed() ), this, 
	       SLOT( seriesArrangerClosed() ) );
      connect( sa, SIGNAL( seriesChanged() ), this, SLOT( updateSeries() ) );
      connect( this, SIGNAL( seriesChanged() ), sa, SLOT( updateView() ) );
      connect( sa, SIGNAL( levelChanged( unsigned ) ), this, 
	       SIGNAL( stageChanged( unsigned ) ) );
      d->editM->setItemChecked( 6, true );
    }
}


void QRMainWin::seriesArrangerClosed()
{
  d->editM->setItemChecked( 6, false );
}


void QRMainWin::updateSeries()
{
  changeLevelMyself( game.tb );
  emit seriesChanged();
}


void QRMainWin::changeLevel( unsigned l )
{
  emit stageChanged( l );
}


QPixmap QRMainWin::originalSprite( unsigned short num ) const
{
#if defined( _WIN32 ) && defined( WIN32_AVOID_PIXMAPS )
  QPixmap	pix;
  pix.convertFromImage( *_osprite[ num ] );
  return( pix );
#else
  return( *_osprite[ num ] );
#endif
}


void QRMainWin::paintStart( int x, int y, bool inlevel, QRPlayField* source )
{
  QREditPalette	*pal = theQRWin->editPalette();
  if( !pal )
    return;

  unsigned  paintmode = pal->drawingMode();
  if( paintmode > d->painters.size() )
    return;

  d->painters[ paintmode ]->start( x, y, inlevel, source );
}


void QRMainWin::paintMove( int x, int y, bool inlevel, QRPlayField* source )
{
  QREditPalette	*pal = theQRWin->editPalette();
  if( !pal )
    return;

  unsigned  paintmode = pal->drawingMode();
  if( paintmode > d->painters.size() )
    return;

  d->painters[ paintmode ]->move( x, y, inlevel, source );
}


void QRMainWin::paintStop( int x, int y, bool inlevel, QRPlayField* source )
{
  QREditPalette	*pal = theQRWin->editPalette();
  if( !pal )
    return;

  unsigned  paintmode = pal->drawingMode();
  if( paintmode > d->painters.size() )
    return;

  d->painters[ paintmode ]->stop( x, y, inlevel, source );
}


const vector<Painter*> & QRMainWin::painters() const
{
  return( d->painters );
}


namespace
{

  string & _lang()
  {
    static string l;
    return l;
  }

}


void QRMainWin::setLanguage( const std::string & l )
{
  _lang() = l;
}


std::string QRMainWin::language()
{
  return _lang();
}


