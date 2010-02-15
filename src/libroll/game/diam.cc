/***************************************************************************
                          diam.cc  -  description
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


#include <roll/game/diam.h>
#include <roll/game/vars.h>

using namespace roll;


RDiam::~RDiam()
{
}


unsigned RDiam::diamNum() const
{
  div_t	sp = div( s & 0xff, 40 );

  if( s < 0xff )
    return( sp.quot );
  else
    return( sp.quot + 6 );
}


GElem* RDiam::transformed() const
{
  return( elFactory.createElem( s+1 ) );
}



