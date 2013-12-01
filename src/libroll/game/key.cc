/***************************************************************************
                          key.cc  -  description
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


#include <roll/game/key.h>
#include <roll/game/vars.h>

using namespace roll;


RKey::~RKey()
{
}


unsigned RKey::keyNum() const
{
  if( s == 190 )	// passe-partout
    return( 6 );
  if( s == 0x1ec )	// clef page 1
    return( 7 );

  if( s < 0x100 )	// page 0
    {
      div_t	sp = div( s & 0xff, 40 );

      if( sp.rem == 36 )
	return( sp.quot );
    }

  out << "warning: type de clef pas bon\n";
  return( 0 );
}


GElem* RKey::transformed() const
{
  div_t	sp = div( s & 0xff, 40 );

  if( sp.quot < 5 )
    return( elFactory.createElem( s+40 ) );
  else return( elFactory.createElem( s-160 ) );
}


unsigned RKey::keyOfNum( unsigned num )
{
  switch( num )
  {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    return num * 40 + 36;
  case 6:
    return 190;
  case 7:
    return 0x1ec;
  default:
    out << "warning, unknown key type " << num << std::endl;
    return 0;
  };
}



