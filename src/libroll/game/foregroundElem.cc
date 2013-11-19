/***************************************************************************
                          foregroundElem.cc  -  description
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


#include <roll/game/foregroundElem.h>
#include <roll/game/vars.h>

using namespace roll;


unsigned short RForegroundElem::backStillSprite( RBack* ) const
{
  return( 255 );
}


unsigned short RForegroundElem::frontStillSprite( RBack* ) const
{
  return s;
}


//	Wall


Wall::~Wall()
{
}


unsigned short Wall::sprite( RBack* ) const
{
  if( s == 151 && game.running ) // invisible wall
    return 255;
  else
    return s;
}


unsigned short Wall::frontStillSprite( RBack* ) const
{
  //    explosion ?
  if( f & EXP1 )
    return 18;
  if( f & EXP2 )
    return 58;

  if( s == 151 && game.running ) // invisible wall
    return 255;
  else
    return s;
}
