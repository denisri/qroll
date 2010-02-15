/***************************************************************************
                          burningBomb.cc  -  description
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


#include <roll/game/burningBomb.h>
#include <roll/game/vars.h>

using namespace roll;


void RBurningBomb::realProcess( unsigned x, unsigned y )
{
  --_timing;
  if( _timing == 0 )	// mÃ¨che Ã©coulÃ©e
    explode( x, y );

  else RFallingElem::realProcess( x, y );
}


unsigned short RBurningBomb::animation() const
{
  return( s + ( (anim & 1) ? 40 : 0 ) );
}


GElem* RBurningBomb::transformed() const
{
  if( _timing & 1 )	// timing impair : Ã©teinds la bombe
    {
      div_t	sp = div( s & 0xff, 40 );
      return( elFactory.createElem( (s & 0x100) | sp.rem 
				    | ( ((int) (sp.quot/3)) * 120 ) ) );
    }
  else			// timing pair: allonge le temps
    {
      RBurningBomb	*b = (RBurningBomb *) elFactory.createElem( s );
      b->setTiming( _timing + 1 );
      return( b );
    }
}


