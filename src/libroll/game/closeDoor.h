/***************************************************************************
                          closeDoor.h  -  description
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


#ifndef ROLL_GAME_CLOSEDOOR_H
#define ROLL_GAME_CLOSEDOOR_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	RCloseDoor: close door */
  class RCloseDoor : public GElem
  {
  public:
    RCloseDoor( unsigned num = 28 );
    ~RCloseDoor();

    virtual bool canBeInBorder() const { return( true ); }

  protected:
    void realProcess( unsigned x, unsigned y );
  };

}

#endif


