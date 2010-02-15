/***************************************************************************
                          keyGen.cc  -  description
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


#include <roll/game/keyGen.h>
#include <roll/game/random.h>


using namespace roll;


unsigned short RKeyGen::chooseElem() const
{
  return( 36 + ((int) (rrand() & 0xff)*6/0x100)*40 );
}


