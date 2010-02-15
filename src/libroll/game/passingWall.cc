/***************************************************************************
                          passingWall.cc  -  description
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


#include <roll/game/passingWall.h>
#include <roll/game/rock.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>

using namespace roll;


void RPassingWall::realProcess( unsigned x, unsigned y )
{
  if( _inside )	// qqchose déjà dedans
    {
      if( testGoOut( x, y ) )
	goOut( x, y );
    }
  else		// rien dedans
    {
      if( y == 0 )
	return;

      GElem	*&gel = game.tbct.d[x][y-1];

      //if( gel->isWaiting() || gel->hasExtFlg() )
	// attendait de pouvoir entrer ?
      if( y > 0 && !gel->isBusy() && testComeIn( gel ) )
	{
	  gel->setWaiting( false );
	  _inside = gel;
	  _inside->f &= ~RETPET;
	  gel = new EmptyElem;
	}

      /* else if( y > 0 && !gel->isBusy() && testComeIn( gel ) )
	 gel->setWaiting( true );	// on te prendra au prochain coup.*/
    }
}


bool RPassingWall::testComeIn( GElem *gel )
{
  if( gel->canFall() && !gel->isFalling()  && !gel->aboutToExplode() )
    {
      if( gel->isMoving() && !( gel->f & WAIT ) )
	{
	  // wait a little bit
	  gel->f = gel->f | WAIT;
	  return( false );
	}
      return( true );
    }
  else return( false );
}


bool RPassingWall::testGoOut( unsigned x, unsigned y )
{
  if( y < game.tbct.sizeY()-1 && game.tbct.d[x][y+1]->isEmpty() )
    return( true );
  else return( false );
}


void RPassingWall::goOut( unsigned x, unsigned y )
{
  delete game.tbct.d[x][y+1];
  _inside->f |= BUSY | BA; //| TOMB | BA;
  game.tbct.d[x][y+1] = _inside;
  /*if( _inside->explodesWhenFalls() && !( game.tbct.d[x][y+2]->isEmpty() ) )
    _inside->f |= RETPET;*/	// désactivé pour compatibilité avec Atari

  /*	pour compatibilité, il faut faire un truc compliqué:
	il ne tombe, en fait, que s'il n'y a rien dessous
	comme ça le rock nitro ne pète pas en sortant du mur, sauf s'il 
	y a un monstre dessous */

  /*if( game.tbct.d[x][y+2]->isEmpty() )
    _inside->f |= TOMB;*/
  if( game.tbct.d[x][y+2]->squashedBy( _inside ) )
    game.tbct.d[x][y+2]->f |= RETPET;
  _inside = 0;
}


