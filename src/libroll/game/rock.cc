/***************************************************************************
                          rock.cc  -  description
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


#include <roll/game/rock.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <roll/game/random.h>


using namespace roll;


RFallingElem::RFallingElem( unsigned num ) : GElem( num )
{
}


RFallingElem::~RFallingElem()
{
}


void RFallingElem::realProcess( unsigned x, unsigned y )
{
  GElem	*under = game.tbct.d[x][y+1];
  if( canEat( under ) )
    {	//	Tombe:
      f &= (~ROUL) & (~TOMB);
      moveDown( x, y );
      return;
    }

  //	Tombait et ne tombe plus: bruit
  if( f & TOMB )
    {
      sound();

      f &= ~(ROUL | TOMB);

      //	Ecrase qqch qui pète?
      if( under->squashedBy( this ) )
	game.tbct.d[x][y+1]->f |= PETE;

      //	Explose en tombant?
      if( explodesWhenFalls() )
	{
	  f &= ~( HAU | BA | GAU | DROI | TOMB );
	  f |= PETE;
	  return;
	}
    }

  f &= ~( ROUL | TOMB | HAU | BA | GAU | DROI );
  if( f & RETPET )
    f = (f & RETPET) | PETE;
  else if( under->isSlippery() )
    {
      if( game.tbct.d[x-1][y]->isEmpty() && game.tbct.d[x-1][y+1]->isEmpty()
	  && (game.tbct.d[x-1][y-1]->f & (TOMB | ROUL) )==0 )
	{	//	Roule gauche:
	  moveLeft( x, y );
	}
      else if( game.tbct.d[x+1][y]->isEmpty() 
	       && game.tbct.d[x+1][y+1]->isEmpty()
	       && ( game.tbct.d[x+1][y-1]->f & (TOMB | ROUL) )==0 )
	{	//	Roule droite:
	  moveRight( x, y );
	}
    }
}


void RFallingElem::moveDown( unsigned x, unsigned y )
{
  delete game.tbct.d[x][y+1];
  game.tbct.d[x][y+1] = this;
  f = ( f & ~(HAU | DROI | GAU | RETPET) ) | BUSY | TOMB | BA;
  game.tbct.d[x][y] = new EmptyElem;
  if( y < game.tbct.sizeY()-2 )
    if( game.tbct.d[x][y+2]->squashedBy( this ) )
      game.tbct.d[x][y+2]->f |= RETPET;
  if( explodesWhenFalls() && !( game.tbct.d[x][y+2]->isEmpty() ) )
    f |= RETPET;
}


void RFallingElem::moveLeft( unsigned x, unsigned y )
{
  delete game.tbct.d[x-1][y];
  game.tbct.d[x-1][y] = this;
  f |= ROUL | GAU;
  game.tbct.d[x][y] = new EmptyElem;
}


void RFallingElem::moveRight( unsigned x, unsigned y )
{
  delete game.tbct.d[x+1][y];
  game.tbct.d[x+1][y] = this;
  f |= ROUL | DROI | BUSY;
  game.tbct.d[x][y] = new EmptyElem;
}


void RFallingElem::sound() const
{
  RRSoundProcessor::processor().process( soundWhenFalls() );
}


//


GElem* Rock::transformed() const
{
  div_t	sp = div( s & 0xff, 40 );
  switch( sp.rem )
    {
    case 38:
      return( elFactory.createElem( s-1 ) );
    case 39:
      switch( sp.quot )
	{
	case 0:
	case 1:
	case 2:
	case 4:
	  return( elFactory.createElem( s+40 ) );
	case 3:
	  return( elFactory.createElem( s-2 ) );
	case 5:
	  return( elFactory.createElem( s-40 ) );
	}
    default:
      return( elFactory.createElem( s ) );
    }
}



//


void RollingRock::moveLeft( unsigned x, unsigned y )
{
  if( (s & 0xff) < 120 )
    s += 40;
  else
    s -= 120;
  Rock::moveLeft( x, y );
}


void RollingRock::moveRight( unsigned x, unsigned y )
{
  if( (s & 0xff) >= 40 )
    s -= 40;
  else
    s += 120;
  Rock::moveRight( x, y );
}



//


RNitroRock::~RNitroRock()
{
}


//


RNitroPlusRock::~RNitroPlusRock()
{
}


//


GElem* RGrassRock::transformed() const
{
  return( elFactory.createElem( s+40 ) );
}


bool RGrassRock::canEat( const GElem* el ) const
{
  return( el->isEmpty() || el->isGrass() 
      || ( el->isJungle() && (rrand() & 0xff) <= game.tbct.strength() ) );
}








