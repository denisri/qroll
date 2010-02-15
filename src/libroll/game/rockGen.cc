/***************************************************************************
                          rockGen.cc  -  description
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


#include <roll/game/rockGen.h>
#include <roll/game/rock.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


void RRockGen::realProcess( unsigned x, unsigned y )
{
  if( y < game.tbct.sizeY()-1 && (rrand() & 0xff) <= game.tbct.genSpeed() )
    {
      GElem	*&down = game.tbct.d[x][y+1];
      if( down->isEmpty() )
	{
	  delete down;
	  down = elFactory.createElem( chooseElem() );
	  down->f |= BUSY | TOMB | BA;
	  if( game.tbct.d[x][y+2]->squashedBy( down ) )
	      game.tbct.d[x][y+2]->f |= RETPET;
	  if( sp_flg[ down->s ].l1 & EXPL_TB 
	      && !( game.tbct.d[x][y+2]->isEmpty() ) )
	    down->f |= RETPET;
	}
    }
}


unsigned short RRockGen::chooseElem() const
{
  return( 38+ ((int) (rrand() & 0xff)*6/0x100)*40 );
}





