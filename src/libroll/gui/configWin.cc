/***************************************************************************
                          configWin.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis RiviÃ¯Â¿Â½e
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

#include <roll/gui/configWin.h>
#include <roll/gui/gameConfig.h>
#include <qtabbar.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <vector>
#include <map>
#include <iostream>
#ifdef ANDROID
# include <GLES/gl.h>
#endif

using namespace roll;
using namespace std;


namespace roll
{

  struct QRConfigWin_Private
  {
    QRConfigWin_Private() : tab( 0 ), glopt( 0 ), dbgtim( 0 ) {}

    unsigned		tab;
    vector<QWidget *>	tabs;
    map<int, unsigned>	tabnum;
    QButtonGroup	*glopt;
    QSpinBox		*dbgtim;
  };

}


QRConfigWin::QRConfigWin( QWidget* parent, const char* name, Qt::WFlags f )
  : QWidget( parent, f ), d( new QRConfigWin_Private )
{
  setObjectName( name );
  GameConfig  & gc = GameConfig::config();

  setWindowTitle( tr( "Rock'n'Roll game configuration" ) );

  QVBoxLayout	*mainlay = new QVBoxLayout( this );
  mainlay->setMargin( 10 );
  mainlay->setSpacing( 10 );
  setAttribute( Qt::WA_DeleteOnClose );
  QTabBar	*tbar = new QTabBar( this );
  mainlay->addWidget( tbar );

  d->tabnum[ tbar->addTab( tr( "Graphics" ) ) ]        = 0;
  d->tabnum[ tbar->addTab( tr( "Sound" ) ) ]           = 1;
  d->tabnum[ tbar->addTab( tr( "Multiplayers" ) ) ]    = 2;
  d->tabnum[ tbar->addTab( tr( "Levels debugger" ) ) ] = 3;

//   QFrame	*ln = new QFrame( this );
//   ln->setObjectName( "line" );
//   ln->setFrameStyle( QFrame::HLine | QFrame::Raised );
//   mainlay->addWidget( ln );

  //	Graphics options tab

  QWidget	*gra = new QWidget( this );
  QVBoxLayout *gral = new QVBoxLayout( gra );
  gral->setSpacing( 5 );
  d->tabs.push_back( gra );
  mainlay->addWidget( gra );

  QCheckBox		*gl 
    = new QCheckBox( tr( "Use OpenGL to render pixmaps" ), gra );
  gral->addWidget( gl );
  gl->setToolTip( tr( " Using OpenGL can make Rock'n'Roll a LOT faster \n"
                      " if you have a properly accelerated OpenGL.\n" 
                      " If your OpenGL is software only (software Mesa or\n"
                      " unsupported card), you probably won't get any\n" 
                      " improvement (it can even be worse than Qt only).\n" 
                      " If this option is disabled, this means R&R has\n" 
                      " been compiled without OpenGL support, this is\n" 
                      " the case if you don't have OpenGL installed" ) );
  QGroupBox *optb = new QGroupBox( tr( "OpenGL options" ), gra );
  gral->addWidget(optb );
  QVBoxLayout *optl = new QVBoxLayout( optb );
  d->glopt = new QButtonGroup( optb );
  QRadioButton	*r
      = new QRadioButton( tr( "Use glDrawPixels" ), optb );
  optl->addWidget( r );
  d->glopt->addButton( r );
  d->glopt->setId( r, 0 );
#ifdef GL_VERSION_ES_CM_1_0
  // no glDrawPixels in GLES, so this mode is disabled
  r->setEnabled( false );
#endif
  r->setToolTip( tr( " This options makes R&R draw graphics via OpenGL \n"
                     " 2D operations. This can improve especially scaled\n" 
                     " images if your OpenGL implementation can\n"
                     " accelerate it, but it may depend on the driver." ) );
  r = new QRadioButton( tr( "Textures and 3D operations" ), optb );
  optl->addWidget( r );
  d->glopt->addButton( r );
  r->setToolTip( tr( " This option makes R&R render graphics as 3D\n"
                     " textured objects. If your graphics card supports\n"
                     " it and has enough memory to hold all textures, it\n"
                     " will probably be the fastest since all graphics\n"
                     " are resident in the card." ) );
  d->glopt->setId( r, 1 );
  gl->setChecked( gc.usesOpenGL() );
  //d->glopt->setEnabled( theQRWin->usesOpenGL() );
#ifdef RR_NO_OPENGL
  gl->setEnabled( false );
  d->glopt->setEnabled( false );
#else
  d->glopt->buttons()[ gc.glMode() ]->setChecked( true );
#endif
  gral->addStretch();

  //	Sound tab

  QWidget *snd = new QWidget( this );
  QVBoxLayout *sndl = new QVBoxLayout( snd );
  snd->hide();
  sndl->setSpacing( 5 );
  d->tabs.push_back( snd );
  mainlay->addWidget( snd );

  sndl->addWidget( new QLabel( tr( "No sound options up to now..." ), snd ) );
  sndl->addStretch();

  //	Multiplayer tab

  QWidget *mul = new QWidget( this );
  QVBoxLayout *mull = new QVBoxLayout( mul );
  mul->hide();
  mull->setSpacing( 5 );
  d->tabs.push_back( mul );
  mainlay->addWidget( mul );

  QCheckBox	*rollexit 
    = new QCheckBox( tr( "All Rolls must get to a door before the level "
			 "finishes" ), mul );
  rollexit->setChecked( gc.allRollsMustExit() );
  mull->addWidget( rollexit );
  mull->addStretch();

  //	Debug tab

  QWidget *dbg = new QWidget( this );
  QVBoxLayout *dbgl = new QVBoxLayout( dbg );
  dbg->hide();
  dbgl->setSpacing( 5 );
  d->tabs.push_back( dbg );
  mainlay->addWidget( dbg );

  QHBoxLayout *dbh = new QHBoxLayout( 0 );
  dbgl->addLayout( dbh );
  QCheckBox	*dbc = new QCheckBox( tr( "Use custom timing (ms) :" ), dbg );
  dbh->addWidget( dbc );
  d->dbgtim = new QSpinBox( dbg );
  d->dbgtim->setRange( 1, 1000 );
  dbh->addWidget( d->dbgtim );
  d->dbgtim->setValue( gc.timing() );
  if( !gc.usesCustomTiming() )
    d->dbgtim->setEnabled( false );
  else
    dbc->setChecked( true );
  dbgl->addStretch();

  // save button

  QPushButton *sb = new QPushButton( tr( "Save config" ), this );
  sb->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  mainlay->addWidget( sb );

  //	Connects

  connect( tbar, SIGNAL( selected( int ) ), this, SLOT( enableTab( int ) ) );
  connect( rollexit, SIGNAL( toggled( bool ) ), this, 
	   SLOT( rollExit( bool ) ) );
  connect( gl, SIGNAL( toggled( bool ) ), this, SLOT( useOpenGL( bool ) ) );
  connect( d->glopt, SIGNAL( buttonClicked( int ) ), this,
	   SLOT( glModeChanged( int ) ) );
  connect( dbc, SIGNAL( toggled( bool ) ), this, 
	   SLOT( useCustomTiming( bool ) ) );
  connect( d->dbgtim, SIGNAL( valueChanged( int ) ), this, 
	   SLOT( setTiming( int ) ) );
  connect( sb, SIGNAL( clicked() ), this, SLOT( saveConfig() ) );
}


QRConfigWin::~QRConfigWin()
{
  emit closed();
  delete d;
}


void QRConfigWin::enableTab( int tabid )
{
  unsigned tab = d->tabnum[ tabid ];
  if( tab != d->tab )
    {
      d->tabs[ d->tab ]->hide();
      d->tab = tab;
      d->tabs[ tab ]->show();
    }
}


void QRConfigWin::rollExit( bool x )
{
  GameConfig::config().setAllRollsMustExit( x );
}


void QRConfigWin::useOpenGL( bool x )
{
  GameConfig::config().setUseOpenGL( x );
#ifndef RR_NO_OPENGL
  cout << "config mode: " << GameConfig::config().glMode() << endl;
  d->glopt->buttons()[ GameConfig::config().glMode() ]->setChecked( true );
#endif
}


void QRConfigWin::glModeChanged( int m )
{
  cout << "m: " << m << endl;
  GameConfig::config().setGLMode( m );
  cout << "config mode: " << GameConfig::config().glMode() << endl;
}


void QRConfigWin::useCustomTiming( bool x )
{
  d->dbgtim->setEnabled( x );
  GameConfig::config().setUseCustomTiming( x );
  if( !x )
    {
      d->dbgtim->setValue( GameConfig::config().timing() );
    }
}


void QRConfigWin::setTiming( int x )
{
  GameConfig::config().setTiming( x );
}


void QRConfigWin::saveConfig()
{
  GameConfig::config().save();
}

