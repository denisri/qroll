/***************************************************************************
                          door.h  -  description
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


#ifndef ROLL_GAME_DOOR_H
#define ROLL_GAME_DOOR_H


#include <roll/game/eatable.h>


namespace roll
{

  /**	OpenDoor: open door */
  class OpenDoor : public REatable
  {
  public:
    OpenDoor( unsigned num = 29 );
    virtual ~OpenDoor();

    virtual bool isDoor() const;
    virtual bool canBeInBorder() const { return( true ); }
  };

}

#endif
