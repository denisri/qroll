/***************************************************************************
                          transformWall.cc  -  description
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


#include <roll/game/transformWall.h>

using namespace roll;


void RTransformWall::goOut( unsigned x, unsigned y )
{
  transform();
  RPassingWall::goOut( x, y );
}


void RTransformWall::transform()
{
  GElem	*newEl = _inside->transformed();

  delete _inside;
  _inside = newEl;
}






