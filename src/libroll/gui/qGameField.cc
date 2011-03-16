/***************************************************************************
                          qGameField.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¯Â¿Â½e
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


#include <roll/gui/qGameField.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qnamespace.h>
#include <qevent.h>
#include <qapplication.h>
#include <iostream>

using namespace std;

QRGameField::QRGameField( const QPixmap * const * sprites, 
			  QWidget *parent, const char *name ) 
  : QWidget( parent ), RGameField(), _pscr( 0 ),
    _sprite( sprites ), _eraseborder( true ), _pwidth( 0 ), _pheight( 0 )
{
  resize( 512, 384 );
  //setBackgroundMode( Qt::NoBackground );
  if( name )
    setObjectName( name );
}


QRGameField::~QRGameField()
{
}


void QRGameField::resizeEvent( QResizeEvent* )
{
  displayFull();
}


void QRGameField::keyPressedEvent( QKeyEvent* key )
{
  cout << "GameField keyPressed\n";
  key->ignore();
}


void QRGameField::keyReleasedEvent( QKeyEvent* key )
{
  cout << "GameField keyReleased\n";
  key->ignore();
}


void QRGameField::paintEvent( QPaintEvent* )
{
  updateScreenQt();
}


void QRGameField::displayFull()
{
  RGameField::displayFull();
  emit displayFullDone( this );
}


void QRGameField::displayFullSlot()
{
  displayFull();
}


void QRGameField::displayHalfSlot()
{
  displayHalf();
}


void QRGameField::resetSlot()
{
  reset();
}


void QRGameField::setupScreen( unsigned w, unsigned h )
{
  if( !_pscr )
    _pscr = new QPixmap( w, h );
  else if( (unsigned) _pscr->width() != w || (unsigned) _pscr->height() != h )
    *_pscr = QPixmap( w, h );
}


void QRGameField::copySprite( unsigned spr, int posx, int posy )
{
  QPainter qp;
  qp.begin( _pscr );
  qp.drawPixmap( posx, posy, *_sprite[spr] );
  qp.end();
}


void QRGameField::updateScreenQt()
{
  QPainter	qp;

  qp.begin( this );
  if( scalingEnabled() )
  {
    float	sc = scaleFactor();

    if( _eraseborder )
    {
      qp.scale( 1, 1 );

      int	w2 = (int) (_pwidth * sc + 0.5), h2 = (int) (_pheight * sc + 0.5);
      if( w2 < width() )
      {
        qp.fillRect( w2, 0, width() - w2, height(),
                     QBrush( QColor( 0, 0, 0 ) ) );
      }
      if( h2 < height() )
      {
        qp.fillRect( 0, h2, width(), height() - h2,
                     QBrush( QColor( 0, 0, 0 ) ) );
      }
    }
    qp.scale( sc, sc );
  }
  else
    qp.scale( 1, 1 );
  qp.drawPixmap( 0, 0, *_pscr );
  qp.end();
}


void QRGameField::updateScreen( bool eraseBorder, int ww, int wh )
{
  _eraseborder = eraseBorder;
  _pwidth = ww;
  _pheight = wh;
  update();
}


void QRGameField::mousePressEvent( QMouseEvent* e )
{
  emit mousePress( e );
}


void QRGameField::mouseReleaseEvent( QMouseEvent* e )
{
  emit mouseRelease( e );
}


void QRGameField::mouseMoveEvent( QMouseEvent* e )
{
  emit mouseMove( e );
}


