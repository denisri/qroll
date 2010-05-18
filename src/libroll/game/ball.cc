/***************************************************************************
                          ball.cc  -  description
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


#include <roll/game/ball.h>
#include <roll/game/vars.h>
#include <roll/game/roll.h>
#include <roll/game/playerServer.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;


RBall::~RBall()
{
}


unsigned short RBall::sprite( RBack* back ) const
{
  unsigned short	spr = REatable::sprite( back );

  if( spr != s )
    return( spr );

  switch( f & BOUGE_N )
    {
    case HAU_N:
      return( spr + 120 );
    case BA_N:
      return( spr + 40 );
    case DROI_N:
      return( spr + 80 );
    default:
      return( spr );
    }
}


unsigned short RBall::backStillSprite( RBack* back ) const
{
  unsigned short	spr = REatable::backStillSprite( back );

  if( spr != s )
    return( spr );

  if( _moving )
    return( 255 );

  switch( f & BOUGE_N )
    {
    case HAU_N:
      return( spr + 120 );
    case BA_N:
      return( spr + 40 );
    case DROI_N:
      return( spr + 80 );
    default:
      return( spr );
    }
}


unsigned short RBall::movingSprite() const
{
  if( !_moving )
    return( 255 );

  unsigned	d = f & BOUGE;		// mvt rÃ©el
  if( !d )
    return( 255 );

  unsigned	p = f & BOUGE_N;	// direction regardÃ©e

  switch( d )
    {
    case HAU:
      if( p & HAU_N )
	return( s + 80 );
      else
	return( s + 120 );
    case BA:
      if( p & BA_N )
	return( s );
      else
	return( s + 40 );
    case DROI:
      if( p & BA_N )
	return( s + 40 );
      else
	return( s + 80 );
    default:
      if (p & GAU_N )
	return( s );
      else
	return( s + 120 );
    }
}


void RBall::realProcess( unsigned x, unsigned y )
{
  if( !_moving )
    return;	// immobile

  int		x2, y2;	// prochaine direction
  unsigned	dir = f & BOUGE_N;

  if( dir & (BA_N | GAU_N) )
    y2 = 1;		// bas
  else
    y2 = -1;		// haut
  if( dir & (HAU_N | GAU_N) )	// composante droite / gauche
    x2 = -1;
  else
    x2 = 1;

  GElem		*&el = game.tbct.d[x+x2][y];	// case horizontale visÃ©e
  unsigned	refres = 0;

  if( !el->isEmpty() )
    {
      refres = reflectCause( x+x2, y );
      x2 = 0;	// pas de chgt de x

      if( _moving )
	switch( dir )
	  {
	  case HAU_N:
	    dir = DROI_N;
	    break;
	  case DROI_N:
	    dir = HAU_N;
	    break;
	  case GAU_N:
	    dir = BA_N;
	    break;
	  default:
	    dir = GAU_N;
	  }
      RRSoundProcessor::processor().process( RollSoundBank::BALL );
    }

  GElem		*&el2 = game.tbct.d[x+x2][y+y2];	// case verticale visÃ©e

  if( _moving )
    {
      if( !el2->isEmpty() )
	{
	  refres = reflectCause( x+x2, y+y2 );
	  y2 = 0;

	  if( _moving )
	    switch( dir )
	      {
	      case HAU_N:
		dir = GAU_N;
		break;
	      case DROI_N:
		dir = BA_N;
		break;
	      case GAU_N:
		dir = HAU_N;
		break;
	      default:
		dir = DROI_N;
	      }
	  else
	    switch( dir )
	      {
	      case HAU_N:
		dir = BA_N;
		break;
	      case DROI_N:
		dir = GAU_N;
		break;
	      case GAU_N:
		dir = DROI_N;
		break;
	      default:
		dir = HAU_N;
	      }
          RRSoundProcessor::processor().process( RollSoundBank::BALL );
	}
    }
  else
    y2 = 0;

  //	mouvement

  if( refres != 2 )
    {
      GElem		*&el3 = game.tbct.d[x+x2][y+y2];	// case finale
      GElem		*&ths = game.tbct.d[x][y];

      f &= ~(BOUGE | BOUGE_N);
      f |= dir;
      if( _moving )
	{
	  if( y2 < 0 )
	    f |= HAU;
	  else if( y2 > 0 )
	    f |= BA;
	  else if( x2 < 0 )
	    f |= GAU;
	  else if( x2 > 0 )
	    f |= DROI;
	}

      if( &ths != &el3 )
	{
	  delete el3;
	  el3 = this;
	  ths = new EmptyElem;

	  if( y2 > 0 || (y2 == 0 && x2 > 0 ) )
	    el3->f |= BUSY;
	}
    }
  else	// suicide
    {
      game.tbct.d[x][y] = new EmptyElem;
      delete this;
    }
}


unsigned RBall::reflectCause( unsigned x, unsigned y )
{
  GElem	*&el = game.tbct.d[x][y];

  if( el->isBlob() )
    {
      _moving = false;
      return( 1 );
    }
  else if( el->isRoll() )
    {
      Roll	*rl = (Roll *) el;
      Player	& pl = (*game.tbct.players)[ rl->num() ];
      if( !pl.ball )
	{
	  _moving = false;
	  pl.ball = true;
	  return( 2 );
	}
    }
  else if( el->isExplosive() )
    {
      _moving = false;
      el->f |= PETE;
      return( 2 );
    }
  return( 0 );	// inconnu, rÃ©flexion normale
}




