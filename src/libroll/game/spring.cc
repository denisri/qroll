/***************************************************************************
                          spring.cc  -  description
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


#include <roll/game/spring.h>
#include <roll/game/vars.h>
#include <roll/game/ball.h>

using namespace roll;


void RSpring::realProcess( unsigned x, unsigned y )
{
  switch( state )
    {
    case WAITING:
      checkExtend( x, y );
      break;
    case EXTENDED_UP:
    case EXTENDED_DOWN:
    case EXTENDED_LEFT:
    case EXTENDED_RIGHT:
      //	reviens au repos
      state = WAITING;
      break;
    default:
      //	efface l'extension
      game.tbct.d[x][y] = new EmptyElem;
      delete this;	// hmm.. oui oui...
      break;
    }
}


void RSpring::checkExtend( unsigned x, unsigned y )
{
  int	dx, dy;

  if( direction == HORIZONTAL )
    {
      dx = -1;
      dy = 0;
    }
  else
    {
      dx = 0;
      dy = -1;
    }

  if( checkExtendDir( x, y, dx, dy ) )
    return;
  checkExtendDir( x, y, -dx, -dy );
}


bool RSpring::checkExtendDir( unsigned x, unsigned y, int dx, int dy )
{
  if( dx*2 + (int) x < 0 || x + dx*2 >= game.tbct.sizeX() 
      || dy*2 + (int) y < 0 || y + dy*2 >= game.tbct.sizeY() )
    return( false );

  GElem	*& enear = game.tbct.d[x+dx][y+dy];

  if( enear->canMove() )
    {
      GElem *& efar = game.tbct.d[x+2*dx][y+2*dy];

      if( efar->isEmpty() )
	{
	  delete efar;
	  efar = enear;
	  if( enear->isBall() && !enear->isMoving() )
	    {
	      ((RBall *) enear)->setState( true );
	      enear->f &= ~(BOUGE | BOUGE_N);
	      if( dx < 0 )
		enear->f |= GAU_N;
	      else if( dx > 0 )
		enear->f |= DROI_N;
	      else if( dy < 0 )
		enear->f |= HAU_N;
	      else
		enear->f |= BA_N;
	    }
	  state = extendDir( dx, dy );
	  enear = new RSpring( s, direction, dir( dx, dy ) );
	  if( dx > 0 || dy > 0 )
	    enear->f |= BUSY;
	  RRSoundProcessor::processor().process( RRSoundProcessor::SPRING );
	  return( true );
	}
    }
  return( false );
}


RSpring::State RSpring::dir( int dx, int dy ) const
{
  if( dx < 0 )
    return( LEFT );
  if( dx > 0 )
    return( RIGHT );
  if( dy < 0 )
    return( UP );
  return( DOWN );
}


RSpring::State RSpring::extendDir( int dx, int dy ) const
{
  if( dx < 0 )
    return( EXTENDED_LEFT );
  if( dx > 0 )
    return( EXTENDED_RIGHT );
  if( dy < 0 )
    return( EXTENDED_UP );
  return( EXTENDED_DOWN );
}


unsigned short RSpring::sprite( RBack* back ) const
{
  unsigned short	spr;

  //	background prioritaire ?
  if( back )
    {
      spr = back->sprite();
      if( spr != 255 )
	return( spr );
    }	// pour l'instant...

  //	explosion ?
  if( f & EXP1 )
    return( 18 );
  if( f & EXP2 )
    return( 58 );

  switch( state )
    {
    case WAITING:
      return( s );
    case EXTENDED_DOWN:
    case UP:
      return( s + 80 );
    case EXTENDED_UP:
    case DOWN:
      return( s + 120 );
    case EXTENDED_RIGHT:
    case LEFT:
      return( s + 120 );
      /*case EXTENDED_LEFT:
	case RIGHT:*/
    default:
      return( s + 160 );
    }
}


unsigned short RSpring::backStillSprite( RBack* back ) const
{
  return( sprite( back ) );
}




