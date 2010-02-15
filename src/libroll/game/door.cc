/***************************************************************************
                          door.cc  -  description
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

#include <roll/game/door.h>


using namespace roll;


OpenDoor::OpenDoor( unsigned num ) : REatable( num )
{
}


OpenDoor::~OpenDoor()
{
}


bool OpenDoor::isDoor() const
{
  return( true );
}


