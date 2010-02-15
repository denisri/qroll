/***************************************************************************
                          foregroundElem.cc  -  description
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


#include <roll/game/foregroundElem.h>

using namespace roll;


unsigned short RForegroundElem::backStillSprite( RBack* ) const
{
  return( 255 );
}


unsigned short RForegroundElem::frontStillSprite( RBack* ) const
{
  return( s );
}


//	Wall


Wall::~Wall()
{
}


