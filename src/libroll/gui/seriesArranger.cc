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
#include <q3popupmenu.h>
#include <QDropEvent>
typedef Q3PopupMenu QPopupMenu;
#ifdef _WIN32
#include <wtypes.h>
#include <windef.h>
#ifdef RGB
#undef RGB
#endif
#endif

using namespace std;
using namespace roll;
#if QT_VERSION >= 0x040000
using namespace Qt;
#endif

SeriesArranger	*SeriesArranger::_theSeriesArranger = 0;


struct SeriesArranger::SeriesArranger_Private
{
  SeriesArranger_Private() : iconv( 0 ), scale( 4 ) {}
  SeriesIconView		*iconv;
  float				scale;
};


SeriesArranger::SeriesArranger( QWidget *parent, const char *name, WFlags f )
  : QMainWindow( parent, name, f ), d( new SeriesArranger_Private )
{
  if( _theSeriesArranger )
    cerr << "Warning, creating a non-unique instance of SeriesArranger\n";
  _theSeriesArranger = this;
  setCaption( tr( "Rock'n'Roll series arranger" ) );
  d->iconv = new SeriesIconView( this );
  setCentralWidget( d->iconv );
  d->iconv->setSelectionMode( QIconView::Extended );
  d->iconv->setArrangement( QIconView::LeftToRight );
  d->iconv->setResizeMode( QIconView::Adjust );

  QMenuBar	*menu = menuBar();
  QPopupMenu	*pop = new QPopupMenu;
  pop->insertItem( tr( "Copy" ), this, SLOT( copy() ), CTRL + Key_C );
  pop->insertItem( tr( "Cut" ), this, SLOT( cut() ), CTRL + Key_X );
  pop->insertItem( tr( "Paste" ), this, SLOT( paste() ), CTRL + Key_V );
  pop->insertItem( tr( "Delete" ), this, SLOT( deleteSelection() ), 
		   Key_Delete );
  menu->insertItem( tr( "Edit" ), pop );

  pop = new QPopupMenu;
  pop->insertItem( tr( "Zoom +" ), this, SLOT( zoomMore() ) );
  pop->insertItem( tr( "Zoom -" ), this, SLOT( zoomLess() ) );
  menu->insertItem( tr( "View" ), pop );

#if QT_VERSION >= 0x040000
  connect( d->iconv, SIGNAL( dropped( QDropEvent*, 
           const Q3ValueList<Q3IconDragItem> & ) ),
           this, SLOT( levelsDropped( QDropEvent*,
                       const Q3ValueList<Q3IconDragItem> & ) ) );
  connect( d->iconv, SIGNAL( doubleClicked( Q3IconViewItem* ) ), 
           this, SLOT( changeLevel( Q3IconViewItem* ) ) );
#else
  connect( d->iconv, SIGNAL( dropped( QDropEvent*, 
				      const QValueList<QIconDragItem> & ) ), 
	   this, SLOT( levelsDropped( QDropEvent*, 
				      const QValueList<QIconDragItem> & ) ) );
  connect( d->iconv, SIGNAL( doubleClicked( QIconViewItem* ) ), 
           this, SLOT( changeLevel( QIconViewItem* ) ) );
#endif

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
  QIconViewItem		*item;

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
#if QT_VERSION >= 0x040000
  QImage        pix( px, py, QImage::Format_RGB32 );
#else
  QPixmap	pix( px, py );
#endif
  unsigned short smax = 0, sp;
  for( y = 0; y<ny; ++y )
    for( x=0; x<nx; ++x )
      if( data[ x+y*nx ] >= smax )
        smax = data[ x+y*nx ];
#if QT_VERSION >= 0x040000
  vector<QImage> imgs( smax+1 );
#else
  vector<QPixmap> imgs( smax+1 );
#endif

  QPainter  paint2;
  paint.begin( &pix, d->iconv );

#if QT_VERSION >= 0x040000
  // set fastest, poorest quality
  paint.setRenderHint( QPainter::Antialiasing, false );
  paint.setRenderHint( QPainter::SmoothPixmapTransform, false );
#endif
  for( y = 0; y<ny; ++y )
    for( x=0; x<nx; ++x )
    {
      sp = data[ x+y*nx ];
      if( imgs[sp].isNull() )
#if QT_VERSION >= 0x040000
      {
        imgs[sp] = QImage( (int) d->scale, (int) d->scale, QImage::Format_RGB32 );
        paint2.begin( &imgs[sp], d->iconv );
        paint2.scale( d->scale / 32, d->scale / 32 );
        paint2.setRenderHint( QPainter::Antialiasing, false );
        paint2.setRenderHint( QPainter::SmoothPixmapTransform, false );
        paint2.drawImage( 0, 0, theQRWin->originalSprite( sp ).convertToImage() );
        paint2.end();
      }
      paint.drawImage( int( x*d->scale ), int( y*d->scale ), imgs[ sp ] );
#else
      {
        imgs[sp] = QPixmap( (int) d->scale, (int) d->scale );
        paint2.begin( &imgs[sp], d->iconv );
        paint2.scale( d->scale / 32, d->scale / 32 );
        paint2.drawPixmap( 0, 0, theQRWin->originalSprite( sp ) );
        paint2.end();
      }
      paint.drawPixmap( int( x*d->scale ), int( y*d->scale ), imgs[ sp ] );
#endif
    }
  paint.end();
  // cout << "pixmap time: " << timefl() - tme << endl;

  //	recolor
#if QT_VERSION >= 0x040000
  QImage                & im = pix;
#else
  QImage		im = pix.convertToImage();
#endif
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

#if QT_VERSION >= 0x040000
  /* QPixmap pix2( QPixmap::fromImage( pix ) );
  cout << "convertFromImage: " << timefl() - tme << endl;
  return pix2; */
  return QPixmap::fromImage( pix );
#else
  pix.convertFromImage( im );

  // cout << "convertFromImage: " << timefl() - tme << endl;

  return pix;
#endif
}


void SeriesArranger::levelsDropped( QDropEvent* e, 
				    const QValueList_QIconDragItem & )
{
  //cout << "levels dropped\n";
  map<unsigned, SimpleLevel>		sl;
  if( !LevelsDrag::decode( e, sl ) )
    {
      //cout << "(can't be decoded)\n";
      e->acceptAction( false );
      return;
    }

  unsigned					i;
  set<unsigned>					levels;
  map<unsigned, SimpleLevel>::const_iterator	isl, esl = sl.end();

  for( isl=sl.begin(); isl!=esl; ++isl )
    levels.insert( isl->first );

  QPoint	pos = d->iconv->mapFromParent( e->pos() );
  //cout << "pos: " << pos.x() << ", " << pos.y() << endl;
  QIconViewItem	*item;
  QRect	rect, rect0;
  bool	first = true;

  for( item=d->iconv->firstItem(), i=0; item; item=item->nextItem(), ++i )
    {
      rect = item->rect();
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

  QDropEvent::Action	action = e->action();
  e->acceptAction();
  if( action == QDropEvent::Move )
    {
      SeriesIconView	*ivw = 0;
      try	// Microsoft VC++ throws an exception if dynamic_cast fails
	{
	  ivw = dynamic_cast<SeriesIconView *>( e->source() );
	}
      catch( exception & )
	{
	}

      /*out << "dropevent source: " << e->source() << endl;
	if( e->source() )
	out << "type: " << e->source()->className() << endl;
	else
	out << "(no source)" << endl;*/
      if( !ivw && e->source() 
	  && e->source()->className() == d->iconv->className() )
	// (if not compiled with RTTI support)
	ivw = (SeriesIconView *) e->source();

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
  QApplication::clipboard()->setData( d->iconv->copySelection() );
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
  QIconViewItem	*item = d->iconv->firstItem(), *item2;
  unsigned	i = 0;
  bool		levelchanged = false;

  while( item )
    if( item->isSelected() )
      {
	item2 = item;
	item = item->nextItem();
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
	  }
      }
    else
      {
	++i;
	item=item->nextItem();
      }
  if( levelchanged )
    emit levelChanged( game.tb );
}


void SeriesArranger::paste()
{
  //cout << "paste\n";
  map<unsigned, SimpleLevel>	sl;
  if( !LevelsDrag::decode( QApplication::clipboard()->data(), sl ) )
    return;
  //cout << "paste " << sl.size() << " levels\n";
  QIconViewItem	*item = d->iconv->currentItem();
  if( item )
    insertLevels( item->index(), sl, item );
  else
    insertLevels( ser->numLevels(), sl, 0 );
}


void SeriesArranger::insertLevels( unsigned insertpos, 
				   const map<unsigned, SimpleLevel> & sl, 
				   QIconViewItem* item )
{
  if( insertpos <= (unsigned) game.tb )
    theQRWin->storeLevel();

  bool	totop = false;
  if( item )
    {
      item = item->prevItem();
      if( !item )
	{
	  totop = true;
	  item = d->iconv->firstItem();
	}
    }
  else
    item = d->iconv->lastItem();

  vector<QIconViewItem *>			items;
  unsigned					j, inum, n = sl.size();
  map<unsigned, SimpleLevel>::const_iterator	isl, esl = sl.end();
  QIconViewItem					*item2;
  unsigned ipos = insertpos;

  item2 = d->iconv->firstItem();

  for( j=0, isl=sl.begin(); isl!=esl; ++isl )
    {
      inum = isl->first;
      if( ser->insertLevel( ipos, isl->second ) < 0 )
	{
	  ipos = ser->numLevels();
	  insertpos = ipos;
	  ser->appendLevel( isl->second );
	}
      ++ipos;
      //	copy icon
      for( ; j<inum; ++j )
	item2 = item2->nextItem();
      //cout << "copy " << item2 << endl;
      items.push_back( item2 );
    }

  QIconViewItem	*itemtodel = 0;
  for( isl=sl.begin(); isl!=esl; ++isl )
      {
	item = new SeriesIconViewItem( d->iconv, item, "", 
				       levelPixmap( isl->second ) );
	item->setDropEnabled( true );
	if( totop )
	  {	// workaround a bug in Qt (can't insert at beginning !)
	    itemtodel = d->iconv->firstItem();
	    new SeriesIconViewItem( d->iconv, item, "", *itemtodel->pixmap() );
	    totop = false;
	  }
      }
  delete itemtodel;

  // renum items texts
  for( j=0, item=d->iconv->firstItem(); item; ++j, item=item->nextItem() )
    item->setText( tr( "Level " ) + QString::number( j ) );

  if( insertpos <= (unsigned) game.tb )
    emit levelChanged( game.tb + n );
}

void SeriesArranger::changeLevel( QIconViewItem* item )
{
  emit levelChanged( item->index() );
}


void SeriesArranger::zoomMore()
{
  d->scale *= 2;
  updateView();
}


void SeriesArranger::zoomLess()
{
  if( d->scale >= 2 )
  {
    d->scale /= 2;
    updateView();
  }
}


