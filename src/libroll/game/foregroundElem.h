/***************************************************************************
                          foregroundElem.h  -  description
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


#ifndef ROLL_GAME_FOREGROUNDELEM_H
#define ROLL_GAME_FOREGROUNDELEM_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Elément de 1er plan (devant en cas de superposition) */
  class RForegroundElem : public GElem
  {
  public:
    RForegroundElem( unsigned num ) : GElem( num ) {}
    virtual ~RForegroundElem() {}

    ///	Sprite de fond (immobile, derrière celui qui bouge)
    virtual unsigned short backStillSprite( RBack* back = 0 ) const;
    ///	Sprite immobile de 1er plan: devant celui qui bouge
    virtual unsigned short frontStillSprite( RBack* back = 0 ) const;
  };


  ///	Walls and other solid elements that can exist in levels borders
  class Wall : public RForegroundElem
  {
  public:
    Wall( unsigned num = 20 ) : RForegroundElem( num ) {}
    virtual ~Wall();

    virtual unsigned short sprite( RBack* back = 0 ) const;
    virtual unsigned short frontStillSprite( RBack* back = 0 ) const;
    virtual bool canBeInBorder() const { return( true ); }
  };

}

#endif

