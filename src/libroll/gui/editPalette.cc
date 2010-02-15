/***************************************************************************
                          editPalatte.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001-2004 by Denis Rivi�e
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

#include <roll/gui/editPalette.h>
#include <roll/gui/mainWin.h>
#include <roll/gui/painter.h>
#include <roll/struct/general.h>
#if QT_VERSION >= 0x040000
#include <q3vbox.h>
#include <q3hbox.h>
#include <Q3HButtonGroup>
#include <q3groupbox.h>
#include <q3buttongroup.h>
#include <q3popupmenu.h>
#include <QScrollArea>
typedef Q3VBox QVBox;
typedef Q3HBox QHBox;
typedef Q3HButtonGroup QHButtonGroup;
typedef Q3PopupMenu QPopupMenu;
#else
#include <qvbox.h>
#include <qhbox.h>
#include <qhbuttongroup.h>
#include <qgroupbox.h>
#include <qscrollview.h>
#include <qbuttongroup.h>
typedef QButtonGroup Q3ButtonGroup;
typedef QGroupBox Q3GroupBox;
#endif
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qpainter.h>


using namespace roll;
using namespace std;
#if QT_VERSION >= 0x040000
using namespace Qt;
#endif


struct QREditPalette::Private
{
  Private() 
    : palette( 0 ), quick( 0 ), modes( 0 ), sel( 0 ), modelabel( 0 ), scl( 0 ),
      scalemenu( 0 ), selected( 255 ), mode( 0 ) {}

  Q3ButtonGroup	*palette;
  QToolBar	*quick;
  QToolBar      *modes;
  QLabel	*sel;
  QLabel        *modelabel;
#if QT_VERSION >= 0x040000
  QScrollArea   *scl;
#else
  QScrollView   *scl;
#endif
  QPopupMenu    *scalemenu;
  int		selected;
  unsigned      mode;
  static float  scale;
};


float QREditPalette::Private::scale = 2;


QREditPalette::QREditPalette( QWidget* parent, const char* name, WFlags f )
  : QMainWindow( parent, name, f ), d( new Private )
{
  setCaption( tr( "Rock'n'Roll Edition tools" ) );
  QVBox	*lay = new QVBox( this );
  setCentralWidget( lay );
  const QPixmap * const * spr = theQRWin->sprites();

#if QT_VERSION >= 0x040000
  d->quick = addToolBar( tr( "Quick selection" ) );
#else
  d->quick = new QToolBar( tr( "Quick selection" ), this );
#endif
  //d->quick->setExclusive( true );
  QToolButton	*emb = new QToolButton( d->quick );
  emb->setUsesBigPixmap( true );
  //emb->setToggleButton( true );
  emb->setPixmap( *spr[255] );
  emb->setFixedSize( emb->sizeHint() );

  QToolButton	*rl = new QToolButton( d->quick );
  rl->setUsesBigPixmap( true );
  //rl->setToggleButton( true );
  rl->setPixmap( theQRWin->originalSprite( 34 ) );
  rl->setFixedSize( rl->sizeHint() );
#if QT_VERSION >= 0x040000
  QAction *a = d->quick->addWidget( emb );
  a->setVisible( true );
  a = d->quick->addWidget( rl );
  a->setVisible( true );
#endif

  //d->quick->addSpace( 0 );

#if QT_VERSION >= 0x040000
  d->modes = addToolBar( tr( "Paint modes" ) );
#else
  d->modes = new QToolBar( tr( "Paint modes" ), this );
#endif
  const vector<Painter *> & painters = theQRWin->painters();
  unsigned i, n = painters.size();
  Painter  *p;
  for( i=0; i<n; ++i )
    {
      p = painters[i];
#if QT_VERSION >= 0x040000
      EditorButton *draw =
#endif
        new EditorButton( i, p->pixmap(), p->name(), p->description(), 
                          this, SLOT( setDrawingMode( unsigned ) ),
                          d->modes );
#if QT_VERSION >= 0x040000
      a = d->modes->addWidget( draw );
      a->setVisible( true );
#endif
    }

  Q3GroupBox	*eb = new Q3GroupBox( 1, Qt::Horizontal, tr( "Edit settings :" ), lay );
  QHBox		*cb = new QHBox( eb );
  cb->setSpacing( 10 );
  d->sel = new QLabel( cb );
  d->sel->setFrameStyle( QFrame::Box | QFrame::Sunken );
  d->sel->setPixmap( *spr[255] );
  d->sel->setFixedSize( d->sel->sizeHint() );
  new QLabel( tr( "Current sprite" ), cb );
  new QLabel( tr( "Current mode :" ), cb );
  d->modelabel = new QLabel( cb );
  d->modelabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
  d->modelabel->setPixmap( painters[0]->pixmap() );
  d->modelabel->setFixedSize( d->modelabel->sizeHint() );

#if QT_VERSION >= 0x040000
  d->scl = new QScrollArea( lay );
  d->palette = new Q3ButtonGroup( 40, Horizontal, d->scl );
  //d->scl->setWidget( d->palette );
#else
  d->scl = new QScrollView( lay );
  d->palette = new Q3ButtonGroup( 40, Horizontal, d->scl->viewport() );
  d->scl->addChild( d->palette );
#endif
  d->palette->setExclusive( true );

  fillPalette();
#if QT_VERSION >= 0x040000
  /* There is a bug in Qt4 (4.1 and 4.2) that makes d->palette->sizeHint() 
  wrong when QScrollArea::setWidget() is called before the child (d->palette) 
  has not already its final size.
  */
  //QSize sh = d->palette->sizeHint();
  //cout << "pal sh: " << sh.width() << " x " << sh.height() << endl;
  d->scl->setWidget( d->palette );
#endif
  connect( emb, SIGNAL( clicked() ), this, SLOT( quickSelect0() ) );
  connect( rl, SIGNAL( clicked() ), this, SLOT( quickSelect1() ) );
  connect( d->palette, SIGNAL( clicked( int ) ), this, 
           SLOT( select( int ) ) );
  d->mode = 1;
  setDrawingMode( 0 );

  QMenuBar *menu = menuBar();
  QPopupMenu *pop = new QPopupMenu;
  pop->insertItem( tr( "1:1" ), this, SLOT( scale1() ), 0, 1 );
  pop->insertItem( tr( "1:2" ), this, SLOT( scale2() ), 0, 2 );
  pop->insertItem( tr( "1:4" ), this, SLOT( scale4() ), 0, 4 );
  pop->insertItem( tr( "1:8" ), this, SLOT( scale8() ), 0, 8 );
  pop->setItemChecked( (int) d->scale, true );
  menu->insertItem( tr( "View" ), pop );
  d->scalemenu = pop;

  /*lay->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, 
				   QSizePolicy::Maximum ) );
  setSizePolicy( QSizePolicy( QSizePolicy::Preferred, 
  QSizePolicy::Maximum ) );*/

  resize( 600, 500 );
}


QREditPalette::~QREditPalette()
{
  emit palDestroyed();
  delete d;
}


void QREditPalette::fillPalette()
{
  unsigned	i, p, n = 240;
  QPushButton	*b;
  QSize         ps = theQRWin->originalSprite( 0 ).size();

  ps /= d->scale;

  for( p=0; p<2; ++p )
    for( i=0; i<n; ++i )
      {
	b = new QPushButton( d->palette );
	if( d->scale == 1 )
	  b->setPixmap( theQRWin->originalSprite( p*256 + i ) );
	else
	  {
	    QPixmap pix( ps );
	    QPainter pr( &pix );
	    pr.scale( 1. / d->scale, 1. / d->scale );
	    pr.drawPixmap( 0, 0, theQRWin->originalSprite( p*256 + i ) );
#if QT_VERSION >= 0x040000
            b->setIconSize( ps );
            b->setIcon( QIcon( pix ) );
#else
	    b->setPixmap( pix );
#endif
	  }
	// b->setFixedSize( b->sizeHint() );
        b->setFixedSize( ps + QSize( 4, 4 ) );
        b->setToggleButton( true );
      }
}


void QREditPalette::select( int x )
{
  if( x >= 240 )
    x += 16;
  d->sel->setPixmap( theQRWin->originalSprite( x ) );
  d->selected = x;
}


void QREditPalette::quickSelect0()
{
  quickSelect( 0 );
}


void QREditPalette::quickSelect1()
{
  quickSelect( 1 );
}


void QREditPalette::quickSelect( int x )
{
  int	s;

  switch( x )
    {
    case 1:
      s = 34;	// roll
      break;
    default:
      s = 255;
    }

  d->sel->setPixmap( theQRWin->originalSprite( s ) );
  d->selected = s;
}


unsigned short QREditPalette::selectedSprite() const
{
  return( (unsigned short) d->selected );
}


void QREditPalette::setDrawingMode( unsigned mode )
{
  const vector<Painter *> & painters = theQRWin->painters();
  if( mode > painters.size() )
    {
      err << "setDrawingMode: Wrong mode " << mode << endl;
      return;
    }
  if( d->mode != mode )
    {
      d->mode = mode;
      d->modelabel->setPixmap( painters[ mode ]->pixmap() );
      emit modeChanged( mode );
    }
}


unsigned QREditPalette::drawingMode() const
{
  return( d->mode );
}


void QREditPalette::scale1()
{
  scale( 1 );
}


void QREditPalette::scale2()
{
  scale( 2 );
}


void QREditPalette::scale4()
{
  scale( 4 );
}


void QREditPalette::scale8()
{
  scale( 8 );
}


void QREditPalette::scale( float x )
{
  if( d->scale == x )
    return;

  menuBar()->setItemChecked( (int) d->scale, false );
  d->scale = x;
  menuBar()->setItemChecked( (int) d->scale, true );
  if( d->palette )
    {
#if QT_VERSION >= 0x040000
      d->scl->takeWidget();
#else
      d->scl->removeChild( d->palette );
#endif
      delete d->palette;
    }
#if QT_VERSION >= 0x040000
  d->palette = new Q3ButtonGroup( 40, Horizontal, d->scl );
  d->palette->setExclusive( true );
  fillPalette();
  //QSize sh = d->palette->sizeHint();
  //cout << "pal sh: " << sh.width() << " x " << sh.height() << endl;
  d->scl->setWidget( d->palette );
#else
  d->palette = new Q3ButtonGroup( 40, Horizontal, d->scl->viewport() );
  d->scl->addChild( d->palette );
  d->palette->setExclusive( true );
  fillPalette();
#endif
  connect( d->palette, SIGNAL( clicked( int ) ), this, 
	   SLOT( select( int ) ) );
  d->palette->show();
  //d->scl->setMaximumHeight( d->palette->sizeHint().height() );
  int sci = (int) d->scale;
  d->scalemenu->setItemChecked( sci, true );
  if( sci != 1 )
    d->scalemenu->setItemChecked( 1, false );
  if( sci != 2 )
    d->scalemenu->setItemChecked( 2, false );
  if( sci != 4 )
    d->scalemenu->setItemChecked( 4, false );
  if( sci != 8 )
    d->scalemenu->setItemChecked( 8, false );
}


// -----------------------------


EditorButton::EditorButton( unsigned num, const QPixmap & pm, 
			    const QString & textLabel, 
			    const QString & grouptext, QObject * receiver, 
			    const char * slot, QToolBar * parent, 
			    const char * name )
  : QToolButton( pm, textLabel, grouptext, 0, 0, parent, name ), 
    _num( num )
{
  connect( this, SIGNAL( clicked() ), SLOT( emitClicked() ) );
  connect( this, SIGNAL( toggled( bool ) ), SLOT( emitToggled( bool ) ) );
  connect( this, SIGNAL( clicked( unsigned ) ), receiver, slot );
  //connect( this, SIGNAL( toggled( bool, unsigned ) ), receiver, slot );
}


EditorButton::~EditorButton()
{
}


void EditorButton::emitClicked()
{
  emit clicked( _num );
}


void EditorButton::emitToggled( bool x )
{
  emit toggled( x, _num );
}


