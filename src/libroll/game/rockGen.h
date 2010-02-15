/***************************************************************************
                          rockGen.h  -  description
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


#ifndef ROLL_GAME_ROCKGEN_H
#define ROLL_GAME_ROCKGEN_H


#include <roll/game/foregroundElem.h>


namespace roll
{
  /**	Rocks Generator */
  class RRockGen : public RForegroundElem
  {
  public:
    RRockGen( unsigned num = 209 ) : RForegroundElem( num ) {}
    virtual ~RRockGen() {}

    virtual bool canBeInBorder() const { return( true ); }

  protected:
    ///
    virtual void realProcess( unsigned x, unsigned y );
    ///
    virtual unsigned short chooseElem() const;
  };

}

#endif


