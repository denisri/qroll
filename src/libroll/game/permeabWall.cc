/***************************************************************************
                          permeabWall.cc  -  description
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


#include <roll/game/permeabWall.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


bool RPermeabWall::testComeIn( GElem* gel )
{
  return( (rrand() & 0xff) <= game.tbct.permeability() 
      && RPassingWall::testComeIn( gel ) );
}




