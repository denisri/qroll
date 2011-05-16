/***************************************************************************
                          gameField.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivière
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


#include <roll/gui/gameField.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <roll/game/playerServer.h>
#include <roll/game/gElem.h>
#include <iostream>
#include <cmath>

using namespace roll;
using namespace std;


RGameField::RGameField() 
  : _xx( 0 ), _yy( 0 ), _player( 0 ), _scale( false ), 
    _scfac( 1 ), _w( 16 ), _h( 12 ), _sclmode( StandardFOV )
{
#ifdef ANDROID
  // on Android, enable the arbitrary scale, used with pinch gesture
  _sclmode = ArbitraryScale;
#endif
}


RGameField::~RGameField()
{
}


void RGameField::reset()
{
  if( _scale )
  {
    if( _sclmode == StandardFOV )
    {
      _w = 16;
      _h = 12;
    }
    else
    {
      float sf = 1.;
      if( _scfac != 0. )
        sf = 1. / _scfac;
      _w = int( ceil( sf * width() / 32 ) );
      _h = int( ceil( sf * height() / 32 ) );
    }
  }
  else
  {
    _w = int( ceil( width() / 32 ) );
    _h = int( ceil( height() / 32 ) );
  }

  Coord	cp;

  if( game.tbct.players->find( _player ) != game.tbct.players->end() )
    cp = (*game.tbct.players)[ _player ].pos;
  else
    cp = Coord( 0, 0 );

  if( _w >= game.tbct.sizeX() )
    _xx = 0;
  else
  {
    _xx = (cp.x > 8 ? cp.x - 8 : 0 );
    if( _xx + _w > game.tbct.sizeX() )
      _xx = (unsigned) game.tbct.sizeX() - _w;
  }
  if( _h >= game.tbct.sizeY() )
    _yy = 0;
  else
  {
    _yy = (cp.y > 6 ? cp.y - 6 : 0 );
    if( _yy + _h > game.tbct.sizeY() )
      _yy = (unsigned) game.tbct.sizeY() - _h;
  }

  // rajuster les tailles
/*  if( !_scale )
  {
    if( (_w << 5) < (unsigned) width() )
      ++_w;
    if( (_h << 5) < (unsigned) height() )
      ++_h;
  }*/
}


unsigned RGameField::uWidth() const
{
  if( _w == 0 || _scale || (_w << 5) == (unsigned) width() )
    return( _w );
  return( _w - 1 );
}


unsigned RGameField::uHeight() const
{
  if( _h == 0 || _scale || (_h << 5) == (unsigned) height() )
    return( _h );
  return( _h - 1 );
}


void RGameField::displayFull()
{
  unsigned	i, j, w, h;
  unsigned	spr;
  float		sc = 1;

  if( _scale )
  {
    if( _sclmode == StandardFOV )
    {
      sc = ((float) height()) / 384;
      float sc2 = ((float) width()) / 512;
      if( sc2 < sc )
        sc = sc2;
      w = 16;
      h = 12;
    }
    else
    {
      sc = _scfac;
      float sf = 1.;
      if( _scfac != 0. )
        sf = 1. / _scfac;
      w = int( ceil( sf * width() / 32 ) );
      h = int( ceil( sf * height() / 32 ) );
    }
  }
  else
  {
    w = int( ceil( width() / 32 ) );
    h = int( ceil( height() / 32 ) );
  }

  _w = w;
  _h = h;
  _scfac = sc;

  int	ww = (w << 5);
  int	wh = (h << 5);

  setupScreen( ww, wh );

  for( j=0; j<h; ++j )
    for( i=0; i<w; ++i )
    {
      if( i+_xx < game.tbct.sizeX() && j+_yy < game.tbct.sizeY() )
        spr = game.tbct.d[i+_xx][j+_yy]->sprite
          ( game.tbct.b[i+_xx][j+_yy] );
      else spr = 255;
      copySprite( spr, i*32, j*32 );
    }

  updateScreen( true, ww, wh );
}


void RGameField::displayHalf()
{
  int		i, j, w, h;
  unsigned	spr;
  int		dx = 0, dy = 0, di = 0, dj = 0, sx, sy;
  unsigned	xx_old = _xx, yy_old = _yy;

  w = _w;
  h = _h;
  /*if( !_scale )
    {
      if( (w << 5) != width() )
	--w;
      if( (h << 5) != height() )
	--h;
	}*/
#ifdef RR_DEBUG
  cout << "w x h : " << w << " x " << h << endl;
#endif

  int	ww = (w << 5);
  int	wh = (h << 5);

  setupScreen( ww, wh );

  //	calcul des nouveaux xx, yy

  Coord	cp;
  
  if( game.tbct.players->find( _player ) != game.tbct.players->end() )
    cp = (*game.tbct.players)[ _player ].pos;
  else
    cp = Coord( 0, 0 );
  int wborder = 5, hborder = 4;
  if( w < 10 )
    wborder = w / 2;
  if( h < 8 )
    hborder = h / 2;

  if( (int)_xx < (int)cp.x-w+wborder+1 && _xx + w < game.tbct.sizeX() )
    ++_xx;
  if( (int)_xx > (int)cp.x-wborder && _xx>0 )
    --_xx;
  if( _xx == xx_old )
  {
    if( (int)_yy < (int)cp.y-h+hborder+1 && _yy + h < game.tbct.sizeY() )
      ++_yy;
    if( (int)_yy > (int)cp.y-hborder && _yy>0 )
      --_yy;
  }

  //	Calcul de l'offset

  if( xx_old < _xx )
    {
      dx += 16;
      di = -1;
    }

  if( xx_old > _xx )
    {
      dx -= 16;
      w += 2;
    }

  if( yy_old < _yy )
    {
      dj = -1;
      dy += 16;
    }

  if( yy_old > _yy )
    {
      dy -= 16;
      h += 2;
    }

  //	Fond fixe

  for( j=dj; j<h; ++j )
    for( i=di; i<w; ++i )
      {
	if( i+_xx < game.tbct.sizeX() && j+_yy < game.tbct.sizeY() )
	  spr = game.tbct.d[i+_xx][j+_yy]->backStillSprite
	    ( game.tbct.b[i+_xx][j+_yy] );
	else spr = 255;
	copySprite( spr, i*32+dx, j*32+dy );
      }

  //	no need to go out of the level

  if( w+_xx > game.tbct.sizeX() )
    w = game.tbct.sizeX() - _xx;
  if( h+_yy > game.tbct.sizeY() )
    h = game.tbct.sizeY() - _yy;

  //	Choses qui bougent

  unsigned fl;

  for( j=dj; j<h; ++j )
    for( i=di; i<w; ++i )
      {
	spr = game.tbct.d[i+_xx][j+_yy]->movingSprite();
	if( spr != 255 )
	  {
	    fl = game.tbct.d[i+_xx][j+_yy]->f;
	    sx = sy = 0;
	    if( fl & GAU )
	      sx = 16;
	    if( fl & DROI )
	      sx = -16;
	    if( fl & HAU )
	      sy = 16;
	    if( fl & BA )
	      sy = -16;
	    copySprite( spr, i*32+dx+sx, j*32+dy+sy );
	  }
      }

  //	1er plan fixe

  for( j=dj; j<h; ++j )
    for( i=di; i<w; ++i )
      {
	spr = game.tbct.d[i+_xx][j+_yy]->frontStillSprite
	  ( game.tbct.b[i+_xx][j+_yy] );
	if( spr != 255 )
	  copySprite( spr, i*32+dx, j*32+dy );
      }

  updateScreen( false );
}


void RGameField::up()
{
  if( _yy > 0 )
    {
      --_yy;
      displayFull();
    }
}


void RGameField::down()
{
  if( _yy + uHeight() < game.tbct.sizeY() )
    {
      ++_yy;
      displayFull();
    }
}


void RGameField::left()
{
  if( _xx > 0 )
    {
      --_xx;
      displayFull();
    }
}


void RGameField::right()
{
  if( _xx + uWidth() < game.tbct.sizeX() )
    {
      ++_xx;
      displayFull();
    }
}


void RGameField::setPos( unsigned x, unsigned y )
{
  if( x != _xx || y != _yy )
    {
      _xx = x;
      _yy = y;
      displayFull();
    }
}


