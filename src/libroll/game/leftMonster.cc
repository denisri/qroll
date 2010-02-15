/***************************************************************************
                          leftMonster.cc  -  description
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


#include <roll/game/leftMonster.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>

using namespace roll;


RLeftMonster::RLeftMonster( unsigned num ) : GElem( num )
{
}


RLeftMonster::~RLeftMonster()
{
}


void RLeftMonster::realProcess( unsigned x, unsigned y )
{
  if( (f & (RETPET | BA)) == RETPET )
    {
      f = ( f & ~(GAU | DROI | HAU | BA) ) | PETE;
      return;
    }
  
  //	check particular case of blob on right
  //	this case is the only one when the blob might not see us here
  //	(only if we go up, in fact)
  GElem	*& rgt = *(&game.tbct.d[x+1][y]);
  if( rgt->isBlob() )
    {
      f = ( f & ~(GAU | DROI | HAU | BA) ) | PETE;
      return;
    }

  move( x, y );
}


void RLeftMonster::moveDir( unsigned x, unsigned y, Direction dir )
{
  if( dir == RIGHT )
    {
      f &= ~( HAU | BA | GAU | DROI );
      if( f & GAU_N )
	{
	  if( !moveUp( x, y ) && !moveLeft( x, y ) )
	    {
	      f = ( f & ~GAU_N ) | BA_N;
	      if( f & RETPET )
		f |= PETE;
	    }
	}
      else if( f & DROI_N )
	{
	  if( !moveDown( x, y ) && !moveRight( x, y ) )
	    {
	      f = ( f & ~DROI_N ) | HAU_N;
	      if(f & RETPET)
		f |= PETE;
	    }
	}
      else if( f & BA_N )
	{
	  if( !moveLeft( x, y ) && !moveDown( x, y ) )
	    {
	      f = ( f & ~BA_N ) | DROI_N;
	      if(f & RETPET)
		f |= PETE;
	    }
	}
      else
	{
	  if( !moveRight( x, y ) && !moveUp( x, y ) )
	    {
	      f = ( f & ~HAU_N ) | GAU_N;
	      if(f & RETPET)
		f |= PETE;
	    }
	}
    }
  else if( dir == LEFT )
    {
      f &= ~( HAU | BA | GAU | DROI );
      if( f & DROI_N )
	{
	  if( !moveUp( x, y ) && !moveRight( x, y ) )
	    {
	      f = (f & ~DROI_N) | BA_N;
	      if( f & RETPET )
		f |= PETE;
	    }
	}
      else if( f & GAU_N )
	{
	  if( !moveDown(x,y) && !moveLeft(x,y) )
	    {
	      f = (f & ~GAU_N) | HAU_N;
	      if( f & RETPET )
		f |= PETE;
	    }
	}
      else if( f & BA_N )
	{
	  if( !moveRight( x, y ) && !moveDown( x, y ) )
	    {
	      f = (f & ~BA_N) | GAU_N;
	      if( f & RETPET )
		f |= PETE;
	    }
	}
      else
	{
	  if( !moveLeft( x, y ) && !moveUp( x, y ) )
	    {
	      f = (f & ~HAU_N) | DROI_N;
	      if( f & RETPET )
		f |= PETE;
	    }
	}
    }
  // sinon (dir==OTHER) : pas de mouvement
}


bool RLeftMonster::moveLeft( unsigned x, unsigned y )
{
  if( !poss( x-1, y ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x-1][y]);

  delete enear;
  enear = this;
  f = (f & ~( RETPET | DROI | HAU | BA | DROI_N | HAU_N | BA_N ))
    | GAU | GAU_N;
  game.tbct.d[x][y] = new EmptyElem;
  // prend un truc sur le coin de la gueule ?
  GElem	*elup = game.tbct.d[x-1][y-1];
  if( elup->isMoving() && squashedBy( elup ) ) f |= PETE;

  return( true );
}


bool RLeftMonster::moveRight( unsigned x, unsigned y )
{
  if( !poss( x+1, y ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x+1][y]);

  delete enear;
  enear = this;
  f = (f & ~( RETPET | GAU | HAU | BA | GAU_N | HAU_N | BA_N ))
    | DROI | DROI_N | BUSY;
  game.tbct.d[x][y] = new EmptyElem;
  // prend un truc sur le coin de la gueule ?
  GElem	*elup = game.tbct.d[x+1][y-1];
  if( elup->isMoving() && squashedBy( elup ) ) f |= PETE;

  return( true );
}


bool RLeftMonster::moveUp( unsigned x, unsigned y )
{
  if( !poss( x, y-1 ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x][y-1]);

  delete enear;
  enear = this;
  f = (f & ~( RETPET | GAU | DROI | BA | GAU_N | DROI_N | BA_N ))
    | HAU | HAU_N;
  game.tbct.d[x][y] = new EmptyElem;
  // prend un truc sur le coin de la gueule ?
  GElem	*elup = game.tbct.d[x][y-2];
  if( elup->isMoving() && squashedBy( elup ) ) f |= PETE;

  return( true );
}


bool RLeftMonster::moveDown( unsigned x, unsigned y )
{
  if( !poss( x, y+1 ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x][y+1]);

  delete enear;
  enear = this;
  f = (f & ~( RETPET | GAU | DROI | HAU | GAU_N | DROI_N | HAU_N ))
    | BA | BA_N | BUSY;
  game.tbct.d[x][y] = new EmptyElem;

  return( true );
}


bool RLeftMonster::poss( unsigned x, unsigned y, int )
{
  if( game.tbct.d[x][y]->isEmpty() )
    return( true );
  return( false );
}





