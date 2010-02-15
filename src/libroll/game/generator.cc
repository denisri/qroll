/***************************************************************************
                          generator.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis RiviÃ¨re
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


#include <roll/game/generator.h>
#include <roll/game/leftMonster.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


void RGenerator::realProcess( unsigned x, unsigned y )
{
  if( x > 0 )
    tryCreate( x-1, y, GAU | GAU_N );
  if( x < game.tbct.sizeX()-1 )
    tryCreate( x+1, y, DROI | DROI_N | BUSY );
  if( y > 0 )
    tryCreate( x, y-1, HAU | HAU_N );
  if( y < game.tbct.sizeY()-1 )
    tryCreate( x, y+1, BA | BA_N | BUSY );
}


void RGenerator::tryCreate( unsigned x, unsigned y, unsigned flags )
{
  GElem	*&el = game.tbct.d[x][y];

  if( el->isEmpty() && (rrand() & 0xff) < game.tbct.genSpeed() )
    {
      delete el;
      el = create();
      el->f |= flags;
    }
}


GElem* RGenerator::create()
{
  return( elFactory.createElem( ((int) (rrand() & 0xff)*6/0x100)*40 ) );
}


GElem* RGenerator2::create()
{
  return( elFactory.createElem( ((int) (rrand() & 0xff)*6/0x100)*40 + 3 ) );
}


GElem* RGenerator3::create()
{
  return( elFactory.createElem( ((int) (rrand() & 0xff)*6/0x100)*40 + 6 ) );
}


GElem* RGenerator4::create()
{
  return( elFactory.createElem( ((int) (rrand() & 0xff)*5/0x100)*40 + 9) );
}



