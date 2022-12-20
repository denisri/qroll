/***************************************************************************
                          seriesArranger.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2001-2002 by Denis RiviÃ¯Â¿Â½e
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

#include <roll/gui/seriesArranger.h>
#include <roll/gui/seriesIconView.h>
#include <roll/gui/mainWin.h>
#include <roll/struct/series.h>
#include <roll/struct/simpleLevel.h>
#include <roll/gui/mainWin.h>
#include <roll/game/vars.h>
#include <qpainter.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <QDropEvent>
#ifdef _WIN32
#include <wtypes.h>
#include <windef.h>
#ifdef RGB
#undef RGB
#endif
#endif

using namespace std;
using namespace roll;
using namespace Qt;

SeriesArranger	*SeriesArranger::_theSeriesArranger = 0;


struct SeriesArranger::SeriesArranger_Private
{
  SeriesArranger_Private() : iconv( 0 ), scale( 4 ) {}
  SeriesIconView		*iconv;
  float				scale;
};


SeriesArranger::SeriesArranger( QWidget *parent, const char *name, 
                                WindowFlags f )
  : QMainWindow( parent, f ), d( new SeriesArranger_Private )
{
  setAttribute( WA_DeleteOnClose, true );
  setObjectName( name );
  if( _theSeriesArranger )
    cerr << "Warning, creating a non-unique instance of SeriesArranger\n";
  _theSeriesArranger = this;
  setWindowTitle( tr( "Rock'n'Roll series arranger" ) );
  d->iconv = new SeriesIconView( this );
  setCentralWidget( d->iconv );
  d->iconv->setSelectionMode( QListWidget::ExtendedSelection );
//   d->iconv->setArrangement( QListWidget::LeftToRight );
  d->iconv->setResizeMode( QListWidget::Adjust );

  QMenuBar	*menu = menuBar();
  QMenu	*pop = new QMenu( tr( "Edit" ) );
  pop->addAction( tr( "Copy" ), this, SLOT( copy() ), CTRL | Key_C );
  pop->addAction( tr( "Cut" ), this, SLOT( cut() ), CTRL | Key_X );
  pop->addAction( tr( "Paste" ), this, SLOT( paste() ), CTRL | Key_V );
  pop->addAction( tr( "Delete" ), this, SLOT( deleteSelection() ), 
		   Key_Delete );
  menu->addMenu( pop );

  pop = new QMenu( tr( "View" ) );
  pop->addAction( tr( "Zoom +" ), this, SLOT( zoomMore() ) );
  pop->addAction( tr( "Zoom -" ), this, SLOT( zoomLess() ) );
  menu->addMenu( pop );

  connect( d->iconv, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), 
           this, SLOT( changeLevel( QListWidgetItem* ) ) );

  resize( 800, 600 );
  updateView();
}


SeriesArranger::~SeriesArranger()
{
  // emit destroyed();
  if( _theSeriesArranger == this )
    _theSeriesArranger = 0;
  else
    cerr << "Warning, SeriesArranger seems to have several instances\n";
}


void SeriesArranger::updateView()
{
  d->iconv->clear();

  // make icons for each level
  unsigned		i, nl = ser->numLevels();
  SimpleLevel		sl;
  SeriesIconViewItem    *item;

  for( i=0; i<nl; ++i )
    {
      //cout << "making level " << i << "...\n";
      ser->unpack( i, sl );
      item = new SeriesIconViewItem( d->iconv, tr( "Level " ) 
				     + QString::number( i ), 
				     levelPixmap( sl ) );
    }
}

// DEBUG
/*
#include <sys/timeb.h>
float timefl()
{
  static time_t t0 = time( 0 );
  struct timeb tme;
  ftime( &tme );
  return double( tme.time - t0 ) + 0.001 * tme.millitm;
}
*/

QPixmap SeriesArranger::levelPixmap( const SimpleLevel & sl )
{
  unsigned short	*data;
  QPainter		paint;
  unsigned		x, y, nx, ny;
  unsigned		px, py;

  nx = sl.sizeX();
  ny = sl.sizeY();
  // float  tme = timefl();
  // cout << "levelPixmap " << nx << " x " << ny << ", time: " << tme << endl;
  data = sl.foreground();
  px = unsigned( nx * d->scale );
  py = unsigned( ny * d->scale );
  QImage        pix( px, py, QImage::Format_RGB32 );
  unsigned short smax = 0, sp;
  for( y = 0; y<ny; ++y )
    for( x=0; x<nx; ++x )
      if( data[ x+y*nx ] >= smax )
        smax = data[ x+y*nx ];
  vector<QImage> imgs( smax+1 );

  QPainter  paint2;
  paint.begin( &pix );
#if QT_VERSION < 0x060000
  paint.initFrom( d->iconv );
#endif

  // set fastest, poorest quality
  paint.setRenderHint( QPainter::Antialiasing, false );
  paint.setRenderHint( QPainter::SmoothPixmapTransform, false );
  for( y = 0; y<ny; ++y )
    for( x=0; x<nx; ++x )
    {
      sp = data[ x+y*nx ];
      if( imgs[sp].isNull() )
      {
        imgs[sp] = QImage( (int) d->scale, (int) d->scale, QImage::Format_RGB32 );
        paint2.begin( &imgs[sp] );
#if QT_VERSION < 0x060000
        paint2.initFrom( d->iconv );
#endif
        paint2.scale( d->scale / 32, d->scale / 32 );
        paint2.setRenderHint( QPainter::Antialiasing, false );
        paint2.setRenderHint( QPainter::SmoothPixmapTransform, false );
        paint2.drawPixmap( 0, 0, theQRWin->originalSprite( sp ) );
        paint2.end();
      }
      paint.drawImage( int( x*d->scale ), int( y*d->scale ), imgs[ sp ] );
    }
  paint.end();
  // cout << "pixmap time: " << timefl() - tme << endl;

  //	recolor
  QImage                & im = pix;
  QRgb			col;
  unsigned char 	gray;
  WorkLevel::RGB	colors[6], modif;
  short			tmp1, tmp2, tmp3;
  // cout << "convertToImage: " << timefl() - tme << endl;

  colors[ WorkLevel::UNCHANGED ] = WorkLevel::RGB( 0, 0, 0 );
  colors[ WorkLevel::ROCK      ] = WorkLevel::convertedColor( sl._colwall );
  colors[ WorkLevel::DIAM      ] = WorkLevel::convertedColor( sl._coldiam );
  colors[ WorkLevel::ROLL      ] = WorkLevel::convertedColor( sl._colroll );
  colors[ WorkLevel::MONSTER   ] = 
    WorkLevel::convertedColor( sl._colleftmonster );
  colors[ WorkLevel::GRASS     ] = WorkLevel::convertedColor( sl._colgrass );

  for( y=0; y<py; ++y )
    for( x=0; x<px; ++x )
      {
	col = im.pixel( x, y );
	gray = max( max( qRed( col ), qGreen( col ) ), qBlue( col ) );
	/*if( QRMainWin::gameColor( col ) == WorkLevel::UNCHANGED )
	  cout << "rock " << x << ", " << y << endl;*/
	modif = colors[ QRMainWin::gameColor( col ) ];
	tmp1 = gray + modif.r;
	if( tmp1 < 0 ) tmp1 = 0;
	if( tmp1 > 255 ) tmp1 = 255;
	tmp2 = gray + modif.g;
	if( tmp2 < 0 ) tmp2 = 0;
	if( tmp2 > 255 ) tmp2 = 255;
	tmp3 = gray + modif.b;
	if( tmp3 < 0 ) tmp3 = 0;
	if( tmp3 > 255 ) tmp3 = 255;
	im.setPixel( x, y, qRgb( tmp1, tmp2, tmp3 ) );
      }

  // cout << "recolor: " << timefl() - tme << endl;

  /* QPixmap pix2( QPixmap::fromImage( pix ) );
  cout << "convertFromImage: " << timefl() - tme << endl;
  return pix2; */
  return QPixmap::fromImage( pix );
}


void SeriesArranger::levelsDropped( QDropEvent* event )
{
//   QByteArray ba = event->mimeData()->data( "Rock'n'Roll/levels-list" );

  //cout << "levels dropped\n";
  map<unsigned, SimpleLevel>		sl;
  if( !LevelsDrag::decode( event->mimeData(), sl ) )
    {
      //cout << "(can't be decoded)\n";
      event->ignore();
      return;
    }

  set<unsigned>					levels;
  map<unsigned, SimpleLevel>::const_iterator	isl, esl = sl.end();

  for( isl=sl.begin(); isl!=esl; ++isl )
    levels.insert( isl->first );

  QPoint	pos = d->iconv->mapFromParent( event->pos() );
  //cout << "pos: " << pos.x() << ", " << pos.y() << endl;
  QListWidgetItem	*item = 0;
  QRect	rect, rect0;
  bool	first = true;
  int i, nitem = d->iconv->count();

  for( i=0; i<nitem; ++i )
    {
      item = d->iconv->item( i );
      rect = d->iconv->itemWidget( item )->rect();
      /*cout << rect.left() << "x" << rect.top() << "-" << rect.right() 
	<< "x" << rect.bottom() << ", ";*/
      if( first )
	{
	  rect0 = rect;
	  first = false;
	}
      else
	{
	  if( rect0.left() >= rect.left() )	// next icon line
	    {
	      if( pos.y() < rect0.bottom() )
		break;
	      rect0 = rect;
	      //cout << "- nextline - \n";
	    }
	  else
	    {
	      rect0.setLeft( rect.left() );
	      rect0.setRight( rect.right() );
	      if( rect0.top() < rect.top() )
		rect0.setTop( rect.top() );
	      if( rect0.bottom() > rect.bottom() )
		rect0.setBottom( rect.bottom() );
	    }
	}
      /*cout << rect0.left() << "x" << rect0.top() << "-" 
	<< rect0.right() << "x" << rect0.bottom() << endl;*/
      if( pos.y() <= rect0.bottom() && pos.x() < rect0.right() )
	break;
    }

  //cout << "insert before item " << i << endl;
  insertLevels( i, sl, item );

  Qt::DropAction	action = event->dropAction();
  event->acceptProposedAction();
  event->accept();
  if( action == Qt::MoveAction )
    {
      SeriesIconView	*ivw = 0;
      try	// Microsoft VC++ throws an exception if dynamic_cast fails
	{
	  ivw = dynamic_cast<SeriesIconView *>( event->source() );
	}
      catch( exception & )
	{
	}

      /*out << "dropevent source: " << event->source() << endl;
	if( event->source() )
	out << "type: " << event->source()->className() << endl;
	else
	out << "(no source)" << endl;*/
      if( !ivw && event->source() 
	  && event->source()->metaObject()->className() 
            == d->iconv->metaObject()->className() )
	// (if not compiled with RTTI support)
	ivw = (SeriesIconView *) event->source();

      if( ivw )
	{
	  if( ivw != d->iconv )
	    i = ivw->count();
	  ivw->levelsMoved( i, levels );
	}
    }
}


void SeriesArranger::copy()
{
  //cout << "copy\n";
  QApplication::clipboard()->setMimeData( d->iconv->copySelection() );
}


void SeriesArranger::cut()
{
  //cout << "cut\n";
  copy();
  deleteSelection();
}


void SeriesArranger::deleteSelection()
{
  //cout << "delete\n";
  QListWidgetItem *item, *item2;
  unsigned	i = 0, nitem = d->iconv->count();
  bool		levelchanged = false;

  while( i < nitem )
  {
    item = d->iconv->item( i );
    if( item->isSelected() )
      {
	item2 = item;
	if( ser->numLevels() > 0 )
	  {
	    delete item2;
	    if( (unsigned) game.tb >= i )
	      {
		theQRWin->storeLevel();
		if( game.tb > 0 )
		  --game.tb;
		levelchanged = true;
	      }
	    ser->deleteLevel( i );
            --nitem;
	  }
      }
    else
      ++i;
  }
  if( levelchanged )
    emit levelChanged( game.tb );
}


void SeriesArranger::paste()
{
  //cout << "paste\n";
  map<unsigned, SimpleLevel>	sl;
  if( !LevelsDrag::decode( QApplication::clipboard()->mimeData(), sl ) )
    return;
  //cout << "paste " << sl.size() << " levels\n";
  QListWidgetItem	*item = d->iconv->currentItem();
  if( item && item->isSelected() )
    insertLevels( d->iconv->currentRow(), sl, item );
  else
    insertLevels( ser->numLevels(), sl, 0 );
}


void SeriesArranger::insertLevels( unsigned insertpos, 
				   const map<unsigned, SimpleLevel> & sl, 
				   QListWidgetItem* )
{
  if( insertpos <= (unsigned) game.tb )
    theQRWin->storeLevel();

  vector<QListWidgetItem*>			items;
  unsigned					j, n = sl.size();
  map<unsigned, SimpleLevel>::const_iterator	isl, esl = sl.end();
  QListWidgetItem				*item;
  unsigned ipos = insertpos;

  for( j=0, isl=sl.begin(); isl!=esl; ++isl )
  {
    if( ser->insertLevel( ipos, isl->second ) < 0 )
    {
      ipos = ser->numLevels();
      insertpos = ipos;
      ser->appendLevel( isl->second );
    }
    item = new SeriesIconViewItem( 0, "", levelPixmap( isl->second ) );
    d->iconv->insertItem( ipos, item );
    item->setFlags( item->flags() | Qt::ItemIsDropEnabled );
    ++ipos;
  }

  // renum items texts
  int c = d->iconv->count();
  for( j=0; j<(unsigned) c; ++j )
    d->iconv->item( j )->setText( tr( "Level " ) + QString::number( j ) );

  if( insertpos <= (unsigned) game.tb )
    emit levelChanged( game.tb + n );
}

void SeriesArranger::changeLevel( QListWidgetItem* item )
{
  emit levelChanged( d->iconv->row( item ) );
}


void SeriesArranger::zoomMore()
{
  d->scale *= 2;
  d->iconv->setIconSize( QSize( d->scale * 75, d->scale * 75 ) );
  updateView();
}


void SeriesArranger::zoomLess()
{
  if( d->scale >= 2 )
  {
    d->scale /= 2;
    d->iconv->setIconSize( QSize( d->scale * 75, d->scale * 75 ) );
    updateView();
  }
}


