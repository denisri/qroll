/***************************************************************************
                          telepWall.cc  -  description
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


#include <roll/game/telepWall.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


bool RTelepWall::testGoOut( unsigned, unsigned )
{
  return( RPassingWall::testGoOut( game.tbct.x_tel, game.tbct.y_tel ) );

  /*if( game.tbct.d[game.tbct.x_tel][game.tbct.y_tel]->isEmpty() )
    return( true );
    else return( false );*/
}


void RTelepWall::goOut( unsigned x, unsigned y )
{
  unsigned xtel = game.tbct.x_tel;
  unsigned ytel = game.tbct.y_tel;
  if( y < ytel || ( y == ytel && x < xtel ) )	// si après dans la gestion
    _inside->f |= BUSY;
  RPassingWall::goOut( game.tbct.x_tel, game.tbct.y_tel );

  /* unsigned xtel = game.tbct.x_tel;
  unsigned ytel = game.tbct.y_tel;
  GElem	*&dest = game.tbct.d[xtel][ytel];
  GElem *&under = game.tbct.d[xtel][ytel+1];
  delete dest;
  _inside->f |= TOMB | BA;
  if( y < ytel || ( y == ytel && x < xtel ) )	// si après dans la gestion
    _inside->f |= BUSY;
  dest = _inside;
  if( sp_flg[ _inside->s ].l1 & EXPL_TB && !( under->isEmpty() ) )
    _inside->f |= RETPET;
  _inside = 0;
  if( under->squashedBy( this ) )
  under->f |= RETPET;*/
}


//


bool RTelepTransWall::testGoOut( unsigned, unsigned )
{
  return( RTransformWall::testGoOut( game.tbct.x_tel, game.tbct.y_tel ) );

  /*if( game.tbct.d[game.tbct.x_tel][game.tbct.y_tel]->isEmpty() )
    return( true );
    else return( false );*/
}


void RTelepTransWall::goOut( unsigned x, unsigned y )
{
  unsigned xtel = game.tbct.x_tel;
  unsigned ytel = game.tbct.y_tel;
  if( y < ytel || ( y == ytel && x < xtel ) )	// si après dans la gestion
    _inside->f |= BUSY;
  RTransformWall::goOut( game.tbct.x_tel, game.tbct.y_tel );

  /*transform();

  unsigned xtel = game.tbct.x_tel;
  unsigned ytel = game.tbct.y_tel;
  GElem	*&dest = game.tbct.d[xtel][ytel];
  GElem *&under = game.tbct.d[xtel][ytel+1];
  delete dest;
  _inside->f |= TOMB | BA;
  if( y < ytel || ( y == ytel && x < xtel ) )	// si après dans la gestion
    _inside->f |= BUSY;
  dest = _inside;
  if( sp_flg[ _inside->s ].l1 & EXPL_TB && !( under->isEmpty() ) )
    _inside->f |= RETPET;
  _inside = 0;
  if( under->squashedBy( dest ) )
  under->f |= RETPET;*/
}


//


bool RTelepPermeabWall::testGoOut( unsigned x, unsigned y )
{
  return( RTelepWall::testGoOut( x, y ) 
	  && (rrand() & 0xff) <= game.tbct.permeability() );
}





