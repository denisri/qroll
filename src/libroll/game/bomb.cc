/***************************************************************************
                          bomb.cc  -  description
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


#include <roll/game/bomb.h>
#include <roll/game/vars.h>

using namespace roll;


unsigned RBomb::bombNum() const
{
  if( s < 0x100 )
    return( 0 );	// bombe grise

  div_t	sp = div( s & 0xff, 40 );

  if( sp.rem == 33 )
    {
      if( sp.quot < 3 )
	return( 1 );	// bombe rouge
      else return( 2 );	// bombe rose
    }
  else
    return( 3 );	// grenade
}


unsigned short RBomb::burningBomb( unsigned num )
{
  switch( num )
    {
    case 0:
      return( 73 );
    case 1:
      return( 0x121 );
    case 2:
      return( 0x199 );
    case 3:
      return( 0x14c );
    default:
      return( 33 );
    }
}


GElem* RBomb::transformed() const
{
  return( elFactory.createElem( s+40 ) );
}




