/***************************************************************************
                          painter.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivière
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

#include <roll/gui/painter.h>
#include <roll/gui/editPalette.h>
#include <roll/gui/mainWin.h>
#include <roll/game/vars.h>
#include <roll/game/gElem.h>
#include <roll/struct/simpleLevel.h>
#include <qpainter.h>
#include <stack>

#include "paint.xpm"
#include "fill.xpm"
#include "rect.xpm"
#include "copy.xpm"
#include "pattern.xpm"

using namespace roll;
using namespace std;


Painter::~Painter()
{
}


void Painter::start( int, int, bool, QRPlayField* )
{
}


void Painter::move( int, int, bool, QRPlayField* )
{
}


void Painter::stop( int, int, bool, QRPlayField* )
{
}


DrawPainter::DrawPainter( QRMainWin* parent ) : Painter( parent )
{
}


DrawPainter::~DrawPainter()
{
}


void DrawPainter::start( int x, int y, bool inlevel, QRPlayField* )
{
  if( !inlevel )
    return;

  unsigned short	olds = game.tbct.d[x][y]->s;
  QREditPalette         *pal = _parent->editPalette();
  if( !pal )
    return;
  GElem			*ng = elFactory.createElem( pal->selectedSprite() );

  if( ng->s != olds	// element has been changed
      && ( ng->canBeInBorder() || ( x > 0 && x < (int) game.tbct.sizeX() - 1 
				    && y > 0 
				    && y < (int) game.tbct.sizeY() - 1 ) ) )
    {
      delete game.tbct.d[x][y];
      game.tbct.d[x][y] = ng;
      game.tbct.setModified( true );
      // handle undo buffer... ? maybe one day
      _parent->stepFull();	// needs redrawing
    }
  else
    delete ng;
}


void DrawPainter::move( int x, int y, bool inlevel, QRPlayField* source )
{
  start( x, y, inlevel, source );
}


QPixmap DrawPainter::pixmap() const
{
  static QPixmap pix( (const char **) paint_xpm );
  return( pix );
}


QString DrawPainter::name() const
{
  static QString name( QREditPalette::tr( "Paint" ) );
  return( name );
}


QString DrawPainter::description() const
{
  static QString 
    name( QREditPalette::tr( "Paint current element with mouse" ) );
  return( name );
}


FillPainter::FillPainter( QRMainWin* parent ) : Painter( parent )
{
}


FillPainter::~FillPainter()
{
}


void FillPainter::start( int x, int y, bool inlevel, QRPlayField* )
{
  if( !inlevel )
    return;

  QREditPalette         *pal = _parent->editPalette();
  if( !pal )
    return;
  unsigned short        tofill = game.tbct.d[x][y]->s;
  unsigned short        color = pal->selectedSprite();
  unsigned short        bg = (tofill == 0 ? 255 : 0 );

  // front propagation algorithm
  //cout << "fill " << x << ", " << y << endl;
  SimpleLevel  sl; // work in a SimpleLevel
  game.tbct.decode( sl, 1 );

  unsigned short *fg = sl.foreground();
  int i, j, nx = sl.sizeX(), ny = sl.sizeY();

  for( j=0; j<ny; ++j )
    for( i=0; i<nx; ++i )
      if( fg[ i + nx*j ] != tofill )
	fg[ i + nx*j ] = bg;

  stack<pair<int,int> > front;
  pair<int, int>        pos;
  GElem                 *el;
  front.push( pair<int,int>( x, y ) );

  while( !front.empty() )
    {
      pos = front.top();
      front.pop();
      x = pos.first;
      y = pos.second;
      el = elFactory.createElem( color );
      if( el->canBeInBorder() 
	  || ( x > 0 && x < (int) game.tbct.sizeX() - 1 
	       && y > 0 && y < (int) game.tbct.sizeY() - 1 ) )
	{
	  delete game.tbct.d[x][y];
	  game.tbct.d[x][y] = el;
	}
      else
	delete el;
      fg[ x + nx * y ] = bg;
      if( x > 0 && fg[ x-1 + nx*y ] == tofill )
	front.push( pair<int,int>( x - 1, y ) );
      if( x < nx - 1 && fg[ x+1 + nx*y ] == tofill )
	front.push( pair<int,int>( x + 1, y ) );
      if( y > 0 && fg[ x + nx * (y-1) ] == tofill )
	front.push( pair<int,int>( x, y - 1 ) );
      if( y < ny - 1 && fg[ x + nx * (y+1) ] == tofill )
	front.push( pair<int,int>( x, y + 1 ) );
    }
  game.tbct.setModified( true );
  _parent->stepFull();
}


QPixmap FillPainter::pixmap() const
{
  static QPixmap pix( (const char **) fill_xpm );
  return( pix );
}


QString FillPainter::name() const
{
  static QString name( QREditPalette::tr( "Fill" ) );
  return( name );
}


QString FillPainter::description() const
{
  static QString 
    name( QREditPalette::tr( "Fill connex surface with current element" ) );
  return( name );
}


struct RectPainter::Private
{
  int startx;
  int starty;
  int currentx;
  int currenty;
  QRPlayField *source;
};


RectPainter::RectPainter( QRMainWin* parent ) 
  : Painter( parent ), d( new RectPainter::Private )
{
}


RectPainter::~RectPainter()
{
  delete d;
}


void RectPainter::start( int x, int y, bool inlevel, QRPlayField* source )
{
  if( !inlevel )
    return;

  d->startx = x;
  d->starty = y;
  move( x, y, inlevel, source );
}


void RectPainter::move( int x, int y, bool, QRPlayField* source )
{
  d->currentx = x;
  d->currenty = y;
  d->source = source;

  connect( source, SIGNAL( displayFullDone( QWidget* ) ), this, 
	   SLOT( displayRect( QWidget * ) ) );

  source->displayFull();
}


void RectPainter::displayRect( QWidget* w )
{
  int x = d->currentx, y = d->currenty, xm, ym;
  if( x < d->startx )
    {
      xm = x;
      x = d->startx;
    }
  else
    xm = d->startx;
  if( y < d->starty )
    {
      ym = y;
      y = d->starty;
    }
  else
    ym = d->starty;

  QPainter p( w );
  QPoint   pos1 = d->source->posOfElement( xm, ym );
  QPoint   pos2 = d->source->posOfElement( x + 1, y + 1 ) - pos1;
  /*cout << "rect: " << pos1.x() << ", " << pos1.y() << ", " << pos2.x() 
    << ", " << pos2.y() << endl;*/
  p.setPen( QColor( 255, 255, 255 ) );
  p.drawRect( pos1.x(), pos1.y(), pos2.x() - 1, pos2.y() - 1 );
}


void RectPainter::stop( int x, int y, bool inlevel, QRPlayField* source )
{
  /*disconnect( source, SIGNAL( displayFullDone( QWidget* ) ), this, 
    SLOT( displayRecxt( QWidget* ) ) );*/
  //disconnect();
  source->disconnect( this, SLOT( displayRect( QWidget *) ) );

  if( !inlevel )
    {
      return;
#if 0
      if( x < 0 )
	x = 0;
      if( y < 0 )
	y = 0;
      if( x >= game.tb.sizeX() )
	x = game.tb.sizeX() - 1;
      if( y >= game.tb.sizeY() )
	y = game.tb.sizeY() - 1;
#endif
    }

  QREditPalette         *pal = _parent->editPalette();
  if( !pal )
    return;
  unsigned short        color = pal->selectedSprite();
  int                   xe, ye, i, j;
  GElem                 *el;

  if( d->startx < x )
    xe = d->startx;
  else
    {
      xe = x;
      x = d->startx;
    }
  if( d->starty < y )
    ye = d->starty;
  else
    {
      ye = y;
      y = d->starty;
    }
  for( j=ye; j<=y; ++j )
    for( i=xe; i<=x; ++i )
      {
	el = elFactory.createElem( color );
	if( el->canBeInBorder() 
	    || ( i > 0 && i < (int) game.tbct.sizeX() - 1 
		 && j > 0 && j < (int) game.tbct.sizeY() - 1 ) )
	  {
	    delete game.tbct.d[i][j];
	    game.tbct.d[i][j] = el;
	  }
	else
	  delete el;
      }
  game.tbct.setModified( true );
  _parent->stepFull();
}


QPixmap RectPainter::pixmap() const
{
  static QPixmap pix( (const char **) rect_xpm );
  return( pix );
}


QString RectPainter::name() const
{
  static QString name( QREditPalette::tr( "Draw rectangle" ) );
  return( name );
}


QString RectPainter::description() const
{
  static QString 
    name( QREditPalette::tr( "Draw rectangles filled with current element" ) );
  return( name );
}


// ----


struct CopyPainter::Private
{
  static SimpleLevel sl;
};


SimpleLevel CopyPainter::Private::sl;


CopyPainter::CopyPainter( QRMainWin* parent ) 
  : RectPainter( parent ), c( new CopyPainter::Private )
{
}


CopyPainter::~CopyPainter()
{
  delete c;
}


void CopyPainter::stop( int x, int y, bool inlevel, QRPlayField* source )
{
  source->disconnect( this, SLOT( displayRect( QWidget *) ) );

  if( !inlevel )
    {
      return;
#if 0
      if( x < 0 )
	x = 0;
      if( y < 0 )
	y = 0;
      if( x >= game.tb.sizeX() )
	x = game.tb.sizeX() - 1;
      if( y >= game.tb.sizeY() )
	y = game.tb.sizeY() - 1;
#endif
    }

  int                   xe, ye, i, j;

  if( d->startx < x )
    xe = d->startx;
  else
    {
      xe = x;
      x = d->startx;
    }
  if( d->starty < y )
    ye = d->starty;
  else
    {
      ye = y;
      y = d->starty;
    }
  // don't copy background yet
  c->sl = SimpleLevel( game.tbct.sizeX(), game.tbct.sizeY(), false );
  ((LevelParam &) c->sl) = game.tbct;
  c->sl.resize( x - xe + 1, y - ye + 1 );
  unsigned short *fg = c->sl.foreground();

  for( j=ye; j<=y; ++j )
    for( i=xe; i<=x; ++i )
      *fg++ = game.tbct.d[i][j]->s;
}


QPixmap CopyPainter::pixmap() const
{
  static QPixmap pix( (const char**) copy_xpm );
  return( pix );
}


QString CopyPainter::name() const
{
  return( QREditPalette::tr( "Copy area" ) );
}


QString CopyPainter::description() const
{
  return( QREditPalette::tr( "Copy area into pattern buffer" ) );
}


const SimpleLevel & CopyPainter::buffer()
{
  return( Private::sl );
}


// --------


PatternPainter::PatternPainter( QRMainWin* parent ) : Painter( parent )
{
}


PatternPainter::~PatternPainter()
{
}


void PatternPainter::start( int x, int y, bool, QRPlayField* )
{
  const SimpleLevel & sl = CopyPainter::buffer();
  int  i, j, nx = x + sl.sizeX(), ny = y + sl.sizeY();
  const unsigned short *fg = sl.foreground();
  GElem *el;
  for( j=y; j<ny; ++j )
    for( i=x; i<nx; ++i, ++fg )
      if( i>= 0 && (unsigned)i<game.tbct.sizeX() 
	  && j>=0 && (unsigned)j<game.tbct.sizeY() )
	{
	  el = elFactory.createElem( *fg );
	  if( el->canBeInBorder() 
	      || ( i > 0 && i < (int) game.tbct.sizeX() - 1 
		   && j > 0 && j < (int) game.tbct.sizeY() - 1 ) )
	    {
	      delete game.tbct.d[i][j];
	      game.tbct.d[i][j] = el;
	    }
	  else
	    delete el;
	}
  game.tbct.setModified( true );
  _parent->stepFull();
}


void PatternPainter::move( int x, int y, bool inlevel, QRPlayField* source )
{
  start( x, y, inlevel, source );
}


QPixmap PatternPainter::pixmap() const
{
  static QPixmap pix( (const char**) pattern_xpm );
  return( pix );
}


QString PatternPainter::name() const
{
  return( QREditPalette::tr( "Draw pattern" ) );
}


QString PatternPainter::description() const
{
  return( QREditPalette::tr( "Draw pattern using buffer copied with "
			     "Copy painter" ) );
}


