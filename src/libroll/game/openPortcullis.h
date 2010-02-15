/***************************************************************************
                          openPortcullis.h  -  description
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


#ifndef ROLL_GAME_OPENPORTCULLIS_H
#define ROLL_GAME_OPENPORTCULLIS_H


#include <roll/game/back.h>


namespace roll
{

  class ROpenPortcullis : public RBack
  {
  public:
    ROpenPortcullis( unsigned num = 0x1e5 ) : RBack( num ) {}
    virtual ~ROpenPortcullis();

  protected:
  };

}

#endif

