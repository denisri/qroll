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
#if QT_VERSION >= 0x040000
#include <q3vbox.h>
#include <q3hbox.h>
#include <Q3VButtonGroup>
typedef Q3VBox QVBox;
typedef Q3HBox QHBox;
typedef Q3VButtonGroup QVButtonGroup;
#else
#include <qvbox.h>
#include <qvbuttongroup.h>
#endif
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
    QVButtonGroup	*glopt;
    QSpinBox		*dbgtim;
  };

}


QRConfigWin::QRConfigWin( QWidget* parent, const char* name, Qt::WFlags f )
  : QWidget( parent, name, f ), d( new QRConfigWin_Private )
{
  GameConfig  & gc = GameConfig::config();

  setCaption( tr( "Rock'n'Roll game configuration" ) );

  QVBoxLayout	*mainlay = new QVBoxLayout( this, 10, 10 );
#if QT_VERSION >= 0x040000
  setAttribute( Qt::WA_DeleteOnClose );
  QTabBar	*tbar = new QTabBar( this );
  mainlay->addWidget( tbar );

  d->tabnum[ tbar->addTab( tr( "Graphics" ) ) ]        = 0;
  d->tabnum[ tbar->addTab( tr( "Sound" ) ) ]           = 1;
  d->tabnum[ tbar->addTab( tr( "Multiplayers" ) ) ]    = 2;
  d->tabnum[ tbar->addTab( tr( "Levels debugger" ) ) ] = 3;
#else
  QTabBar	*tbar = new QTabBar( this, "panels" );
  mainlay->addWidget( tbar );

  d->tabnum[ tbar->addTab( new QTab( tr( "Graphics" ) ) ) ]        = 0;
  d->tabnum[ tbar->addTab( new QTab( tr( "Sound" ) ) ) ]           = 1;
  d->tabnum[ tbar->addTab( new QTab( tr( "Multiplayers" ) ) ) ]    = 2;
  d->tabnum[ tbar->addTab( new QTab( tr( "Levels debugger" ) ) ) ] = 3;
#endif

  QFrame	*ln = new QFrame( this, "line" );
  ln->setFrameStyle( QFrame::HLine | QFrame::Raised );
  mainlay->addWidget( ln );

  //	Graphics options tab

  QVBox	*gra = new QVBox( this );
  gra->setSpacing( 5 );
  d->tabs.push_back( gra );
  mainlay->addWidget( gra );

  QCheckBox		*gl 
    = new QCheckBox( tr( "Use OpenGL to render pixmaps" ), gra );
  QToolTip::add( gl, tr( " Using OpenGL can make Rock'n'Roll a LOT faster \n"
			 " if you have a properly accelerated OpenGL.\n" 
			 " If your OpenGL is software only (old Mesa or\n"
			 " unsupported card), you probably won't get any\n" 
			 " improvement (it can even be worse than Qt only).\n" 
			 " If this option is disabled, this means R&R has\n" 
			 " been compiled without OpenGL support, this is\n" 
			 " the case if you don't have OpenGL installed" ) );
  d->glopt = new QVButtonGroup( tr( "OpenGL options" ), gra );
  QRadioButton	*r
      = new QRadioButton( tr( "Use glDrawPixels" ), d->glopt );
  QToolTip::add( r, tr( " This options makes R&R draw graphics via OpenGL \n" 
			" 2D operations. This can improve especially scaled\n" 
			" images if your OpenGL implementation can\n" 
			" accelerate it, but for instance on my TNT2 with\n" 
			" today Nvidia's driver, it doesn't at all - it's\n" 
			" even far slower than Qt. It's the best option\n" 
			" with Sun Elite workstations" ) );
  r = new QRadioButton( tr( "Textures & 3D operations" ), d->glopt );
  QToolTip::add( r, tr( " This option makes R&R render graphics as 3D\n" 
			" textured objects. If your graphics card supports\n" 
			" it and has enough memory to hold all textures, it\n" 
			" will probably be the fastest since all graphics\n" 
			" are resident in the card. As a counterpart, some\n" 
			" cards / OpenGL implementations will only render \n" 
			" white squares for some or all of the graphics.\n" 
			" On my TNT2 with the current driver I get almost\n" 
			" zero CPU usage but graphics are mainly white\n" 
			" sqares at the beginning; as I play more and more\n" 
			" elements become visible so if I play long it's\n" 
			" fine !" ) );
  gl->setChecked( gc.usesOpenGL() );
  //d->glopt->setEnabled( theQRWin->usesOpenGL() );
#ifdef RR_NO_OPENGL
  gl->setEnabled( false );
  d->glopt->setEnabled( false );
#else
  d->glopt->setButton( gc.glMode() );
#endif

  //	Sound tab

  QVBox	*snd = new QVBox( this );
  snd->hide();
  snd->setSpacing( 5 );
  d->tabs.push_back( snd );
  mainlay->addWidget( snd );

  new QLabel( tr( "No sound options up to now..." ), snd );

  //	Multiplayer tab

  QVBox	*mul = new QVBox( this );
  mul->hide();
  mul->setSpacing( 5 );
  d->tabs.push_back( mul );
  mainlay->addWidget( mul );

  QCheckBox	*rollexit 
    = new QCheckBox( tr( "All Rolls must get to a door before the level "
			 "finishes" ), mul );
  rollexit->setChecked( gc.allRollsMustExit() );

  //	Debug tab

  QVBox	*dbg = new QVBox( this );
  dbg->hide();
  dbg->setSpacing( 5 );
  d->tabs.push_back( dbg );
  mainlay->addWidget( dbg );

  QHBox	*dbh = new QHBox( dbg );
  QCheckBox	*dbc = new QCheckBox( tr( "Use custom timing (ms) :" ), dbh );
  d->dbgtim = new QSpinBox( 1, 1000, 1, dbh );
  d->dbgtim->setValue( gc.timing() );
  if( !gc.usesCustomTiming() )
    d->dbgtim->setEnabled( false );
  else
    dbc->setChecked( true );

  // save button

  QPushButton *sb = new QPushButton( tr( "Save config" ), this );
  sb->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  mainlay->addWidget( sb );

  //	Connects

  connect( tbar, SIGNAL( selected( int ) ), this, SLOT( enableTab( int ) ) );
  connect( rollexit, SIGNAL( toggled( bool ) ), this, 
	   SLOT( rollExit( bool ) ) );
  connect( gl, SIGNAL( toggled( bool ) ), this, SLOT( useOpenGL( bool ) ) );
  connect( d->glopt, SIGNAL( clicked( int ) ), this, 
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
  d->glopt->setButton( GameConfig::config().glMode() );
#endif
}


void QRConfigWin::glModeChanged( int m )
{
  GameConfig::config().setGLMode( m );
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

