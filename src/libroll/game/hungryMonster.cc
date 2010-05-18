/***************************************************************************
                          hungryMonster.cc  -  description
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


#include <roll/game/hungryMonster.h>
#include <roll/game/vars.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;


RHungryMonster::RHungryMonster( unsigned num ): RRightMonster( num )
{
  f |= DROI_N;
}


RHungryMonster::~RHungryMonster()
{
}


void RHungryMonster::moveDir( unsigned x, unsigned y, Direction dir )
{
  if( dir == RIGHT )
    {
      f &= ~( HAU | BA | GAU | DROI );
      if( f & GAU_N )
	{
	  if( !moveEatUp( x, y ) && !moveEatLeft( x, y ) ) // peut manger ?
	    {
	      if( possEat( x, y+1 ) || possEat( x+1, y ) ) // voit Ã  manger ?
		{	// oui: attends de te tourner du bon cÃ´tÃ©
		  f = ( f & ~GAU_N ) | BA_N;
		  if( f & RETPET )
		    f |= PETE;
		}
	      else	// rien Ã  manger: mvt normal
		RRightMonster::moveDir( x, y, dir );
	    }
	}
      else if( f & DROI_N )
	{
	  if( !moveEatDown( x, y ) && !moveEatRight( x, y ) )
	    {
	      if( possEat( x, y-1 ) || possEat( x-1, y ) )
		{
		  f = ( f & ~DROI_N ) | HAU_N;
		  if(f & RETPET)
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
      else if( f & BA_N )
	{
	  if( !moveEatLeft( x, y ) && !moveEatDown( x, y ) )
	    {
	      if( possEat( x+1, y ) || possEat( x, y-1 ) )
		{
		  f = ( f & ~BA_N ) | DROI_N;
		  if(f & RETPET)
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
      else
	{
	  if( !moveEatRight( x, y ) && !moveEatUp( x, y ) )
	    {
	      if( possEat( x-1, y ) || possEat( x, y+1 ) )
		{
		  f = ( f & ~HAU_N ) | GAU_N;
		  if(f & RETPET)
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
    }
  else if( dir == LEFT )
    {
      f &= ~( HAU | BA | GAU | DROI );
      if( f & DROI_N )
	{
	  if( !moveEatUp( x, y ) && !moveEatRight( x, y ) )
	    {
	      if( possEat( x, y+1 ) || possEat( x-1, y ) )
		{
		  f = (f & ~DROI_N) | BA_N;
		  if( f & RETPET )
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
      else if( f & GAU_N )
	{
	  if( !moveEatDown(x,y) && !moveEatLeft(x,y) )
	    {
	      if( possEat( x, y-1 ) || possEat( x+1, y ) )
		{
		  f = (f & ~GAU_N) | HAU_N;
		  if( f & RETPET )
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
      else if( f & BA_N )
	{
	  if( !moveEatRight( x, y ) && !moveEatDown( x, y ) )
	    {
	      if( possEat( x-1, y ) || possEat( x, y-1 ) )
		{
		  f = (f & ~BA_N) | GAU_N;
		  if( f & RETPET )
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
      else
	{
	  if( !moveEatLeft( x, y ) && !moveEatUp( x, y ) )
	    {
	      if( possEat( x+1, y ) || possEat( x, y+1 ) )
		{
		  f = (f & ~HAU_N) | DROI_N;
		  if( f & RETPET )
		    f |= PETE;
		}
	      else RRightMonster::moveDir( x, y, dir );
	    }
	}
    }
  // sinon (dir==OTHER) : pas de mouvement
}


bool RHungryMonster::moveEatLeft( unsigned x, unsigned y )
{
  if( !possEat( x-1, y ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x-1][y]);
  RRSoundProcessor::processor().process( soundWhenEats() );

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


bool RHungryMonster::moveEatRight( unsigned x, unsigned y )
{
  if( !possEat( x+1, y ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x+1][y]);
  RRSoundProcessor::processor().process( soundWhenEats() );

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


bool RHungryMonster::moveEatUp( unsigned x, unsigned y )
{
  if( !possEat( x, y-1 ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x][y-1]);
  RRSoundProcessor::processor().process( soundWhenEats() );

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


bool RHungryMonster::moveEatDown( unsigned x, unsigned y )
{
  if( !possEat( x, y+1 ) ) return( false );

  GElem	*& enear = *(&game.tbct.d[x][y+1]);
  RRSoundProcessor::processor().process( soundWhenEats() );

  delete enear;
  enear = this;
  f = (f & ~( RETPET | GAU | DROI | HAU | GAU_N | DROI_N | HAU_N ))
    | BA | BA_N | BUSY;
  game.tbct.d[x][y] = new EmptyElem;

  return( true );
}


bool RHungryMonster::possEat( unsigned x, unsigned y )
{
  if( x == 0 || y == 0 || x == game.tbct.sizeX()-1 
      || y == game.tbct.sizeY()-1 )
    return( false );	// mange pas les bords !

  GElem		*&el = game.tbct.d[x][y];
  unsigned	spr = el->s;
  
  if( (sp_flg[s].l2 & INDES) || (el->f & INDE) || el->isEmpty() )
    return( false );

  div_t		sp = div( spr & 0xff, 40 );

  if( spr < 0x100 )
    {
      if( ( sp.rem >= 20 && sp.rem <=26 ) || ( sp.rem == 31 ) )
	return( false );
    }
  else	// page 1
    {
      if( ( sp.rem < 3 && sp.quot >=4 ) 
	  || ( sp.rem >= 6 && sp.rem <=8 && sp.quot >= 2 ) 
	  || ( sp.rem >= 9 && sp.rem <=11 ) 
	  || ( sp.rem == 15 ) 
	  || ( sp.rem >= 20 && sp.rem <= 23 )
	  || ( sp.rem >= 24 && sp.rem <= 25 && sp.quot >=4 ) 
	  || ( sp.rem == 26 && sp.quot <= 4 )
	  )
	return( false );
    }

  return( true );
}


