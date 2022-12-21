/***************************************************************************
                          editPalatte.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001-2004 by Denis RiviÃ¯Â¿Â½e
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
#include <qlayout.h>
#include <QButtonGroup>
#include <qgroupbox.h>
#include <QScrollArea>
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qpainter.h>
#include <math.h>


using namespace roll;
using namespace std;
using namespace Qt;


struct QREditPalette::Private
{
  Private() 
    : palette( 0 ), palettebgp( 0 ), pallay( 0 ), quick( 0 ), modes( 0 ), 
      sel( 0 ), 
      modelabel( 0 ),  scl( 0 ), scalemenu( 0 ), selected( 255 ), mode( 0 ) 
  {}

  QGroupBox     *palette;
  QButtonGroup	*palettebgp;
  QGridLayout   *pallay;
  QToolBar	*quick;
  QToolBar      *modes;
  QLabel	*sel;
  QLabel        *modelabel;
  QScrollArea   *scl;
  QMenu         *scalemenu;
  int           selected;
  unsigned      mode;
  static float  scale;
  vector<QAction *> scaleactions;
};


float QREditPalette::Private::scale = 2;


QREditPalette::QREditPalette( QWidget* parent, const char* name, 
                              WindowFlags f )
  : QMainWindow( parent, f ), d( new Private )
{
  setWindowTitle( tr( "Rock'n'Roll Edition tools" ) );
  setObjectName( name );
  setAttribute( Qt::WA_DeleteOnClose );

  QWidget *cwid = new QWidget( this );
  QVBoxLayout	*lay = new QVBoxLayout( cwid );
  setCentralWidget( cwid );
  const QPixmap * const * spr = theQRWin->sprites();

  d->quick = addToolBar( tr( "Quick selection" ) );
  QToolButton	*emb = new QToolButton( d->quick );
  emb->setIconSize( QSize( 32, 32 ) );
  emb->setIcon( QIcon( *spr[255] ) );
  emb->setFixedSize( emb->sizeHint() );

  QToolButton	*rl = new QToolButton( d->quick );
  rl->setIconSize( QSize( 32, 32 ) );
  //rl->setToggleButton( true );
  rl->setIcon( QIcon( theQRWin->originalSprite( 34 ) ) );
  rl->setFixedSize( rl->sizeHint() );
  QAction *a = d->quick->addWidget( emb );
  a->setVisible( true );
  a = d->quick->addWidget( rl );
  a->setVisible( true );

  //d->quick->addSpace( 0 );

  d->modes = addToolBar( tr( "Paint modes" ) );
  const vector<Painter *> & painters = theQRWin->painters();
  unsigned i, n = painters.size();
  Painter  *p;
  for( i=0; i<n; ++i )
    {
      p = painters[i];
      EditorButton *draw =
        new EditorButton( i, p->pixmap(), p->name(), p->description(), 
                          this, SLOT( setDrawingMode( unsigned ) ),
                          d->modes );
      a = d->modes->addWidget( draw );
      a->setVisible( true );
    }

  QGroupBox *eb = new QGroupBox( tr( "Edit settings :" ), cwid );
  lay->addWidget( eb );
  QHBoxLayout *cb = new QHBoxLayout( eb );
  cb->setSpacing( 10 );
  d->sel = new QLabel( eb );
  cb->addWidget( d->sel );
  d->sel->setFrameStyle( QFrame::Box | QFrame::Sunken );
  d->sel->setPixmap( *spr[255] );
  d->sel->setFixedSize( d->sel->sizeHint() );
  cb->addWidget( new QLabel( tr( "Current sprite" ), eb ) );
  cb->addWidget( new QLabel( tr( "Current mode :" ), eb ) );
  d->modelabel = new QLabel( eb );
  cb->addWidget( d->modelabel );
  d->modelabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
  d->modelabel->setPixmap( painters[0]->pixmap() );
  d->modelabel->setFixedSize( d->modelabel->sizeHint() );

  d->scl = new QScrollArea( cwid );
  lay->addWidget( d->scl );
  d->palette = new QGroupBox( d->scl );
  d->pallay = new QGridLayout( d->palette );
  d->pallay->setContentsMargins( 3, 3, 3, 3 );
  d->pallay->setSpacing( 2 );
  d->palettebgp = new QButtonGroup( d->palette );
  d->palettebgp->setExclusive( true );

  fillPalette();
  /* There is a bug in Qt4 (4.1 and 4.2) that makes d->palette->sizeHint() 
  wrong when QScrollArea::setWidget() is called before the child (d->palette) 
  has not already its final size.
  */
  //QSize sh = d->palette->sizeHint();
  //cout << "pal sh: " << sh.width() << " x " << sh.height() << endl;
  d->scl->setWidget( d->palette );
  connect( emb, SIGNAL( clicked() ), this, SLOT( quickSelect0() ) );
  connect( rl, SIGNAL( clicked() ), this, SLOT( quickSelect1() ) );
#if QT_VERSION >= 0x050f00
  connect( d->palettebgp, SIGNAL( idClicked( int ) ), this,
           SLOT( select( int ) ) );
#else
  connect( d->palettebgp, SIGNAL( buttonClicked( int ) ), this,
           SLOT( select( int ) ) );
#endif
  d->mode = 1;
  setDrawingMode( 0 );

  QMenuBar *menu = menuBar();
  QMenu *pop = new QMenu( tr( "View" ) );
  d->scaleactions.resize( 4 );
  d->scaleactions[0] = pop->addAction( tr( "1:1" ), this, SLOT( scale1() ) );
  d->scaleactions[1] = pop->addAction( tr( "1:2" ), this, SLOT( scale2() ) );
  d->scaleactions[2] = pop->addAction( tr( "1:4" ), this, SLOT( scale4() ) );
  d->scaleactions[3] = pop->addAction( tr( "1:8" ), this, SLOT( scale8() ) );
  for( int i=0; i<4; ++i )
    d->scaleactions[i]->setCheckable( true );
  d->scaleactions[ int( rint( log( d->scale ) / log(2) ) ) ]->setChecked(
    true );
  menu->addMenu( pop );
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
        d->palettebgp->addButton( b );
        d->palettebgp->setId( b, p*256 + i );
        d->pallay->addWidget( b, int( i / 40 ) + p * 6, i % 40 );
	if( d->scale == 1 )
        {
          b->setIconSize( ps );
          b->setIcon( QIcon( theQRWin->originalSprite( p*256 + i ) ) );
        }
	else
	  {
	    QPixmap pix( ps );
	    QPainter pr( &pix );
	    pr.scale( 1. / d->scale, 1. / d->scale );
	    pr.drawPixmap( 0, 0, theQRWin->originalSprite( p*256 + i ) );
            b->setIconSize( ps );
            b->setIcon( QIcon( pix ) );
	  }
	// b->setFixedSize( b->sizeHint() );
        b->setFixedSize( ps + QSize( 4, 4 ) );
        b->setCheckable( true );
      }
}


void QREditPalette::select( int x )
{
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

  int intscl = int( rint( log( x ) / log( 2 ) ) );

  d->scaleactions[ int( rint( log( d->scale ) / log( 2 ) ) ) ]->setChecked( 
    false );
  d->scale = x;
  d->scaleactions[ intscl ]->setChecked( true );
  if( d->palette )
  {
    d->scl->takeWidget();
    delete d->palettebgp;
    delete d->palette;
  }
  d->palette = new QGroupBox( d->scl );
  d->pallay = new QGridLayout( d->palette );
  d->pallay->setContentsMargins( 3, 3, 3, 3 );
  d->pallay->setSpacing( 2 );
  d->palettebgp = new QButtonGroup( d->palette );
  d->palettebgp->setExclusive( true );
  fillPalette();
  //QSize sh = d->palette->sizeHint();
  //cout << "pal sh: " << sh.width() << " x " << sh.height() << endl;
  d->scl->setWidget( d->palette );
#if QT_VERSION >= 0x050f00
  connect( d->palettebgp, SIGNAL( idClicked( int ) ), this,
           SLOT( select( int ) ) );
#else
  connect( d->palettebgp, SIGNAL( buttonClicked( int ) ), this,
           SLOT( select( int ) ) );
#endif
  d->palette->show();
}


// -----------------------------


EditorButton::EditorButton( unsigned num, const QPixmap & pm, 
			    const QString & textLabel, 
			    const QString & grouptext, QObject * receiver, 
			    const char * slot, QToolBar * parent, 
			    const char * name )
  : QToolButton( parent ), _num( num )
{
  setObjectName( name );
  setIcon( QIcon( pm ) );
  setText( textLabel );
  setToolTip( grouptext );
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


