/***************************************************************************
                          button.h  -  description
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


#ifndef ROLL_GAME_TRIGGERCOVER_H
#define ROLL_GAME_TRIGGERCOVER_H


#include <roll/game/foregroundElem.h>
#include <roll/game/vars.h>

namespace roll
{

  class RTriggerCover : public RForegroundElem
  {
  public:
    RTriggerCover( unsigned short s ) : RForegroundElem( s ) {}
    virtual ~RTriggerCover();
    virtual bool isExplosive() const { out << "explosive RTriggerCover\n"; return true; }

  protected:
    virtual void realProcess( unsigned x, unsigned y );
  };

}

#endif

