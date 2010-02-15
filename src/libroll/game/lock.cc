/***************************************************************************
                          lock.cc  -  description
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


#include <roll/game/lock.h>
#include <roll/game/key.h>
#include <roll/game/vars.h>

using namespace roll;


RLock::~RLock()
{
}


unsigned RLock::lockNum() const
{
  if( s < 0x100 )	// page 0
    {
      div_t	sp = div( s & 0xff, 40 );

      return( sp.quot );
    }

  out << "warning: type de serrure pas bon\n";
  return( 6 );
}


bool RLock::canBeOpened( unsigned key ) const
{
  unsigned loc = lockNum();

  if( key == 6 && loc < 6 )	// passe-partout et serrure classique
    return( true );

  if( key < 6 && loc == key )	// serrures normales
    return( true );

  return( false );
}




