/***************************************************************************
                          conveyorBelt.h  -  description
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


#ifndef ROLL_GAME_CONVEYORBELT_H
#define ROLL_GAME_CONVEYORBELT_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Tapis roulant */
  class RConveyorBelt : public GElem
  {
  public:
    RConveyorBelt( unsigned num = 0x1a0, unsigned char nb = 0 ) 
      : GElem( num ), _num( nb ) {}
    virtual ~RConveyorBelt() {}

    ///	Animation spéciale (dépend de la direction)
    virtual unsigned short animation() const;
    virtual bool canBeInBorder() const { return( true ); }

  protected:
    virtual void realProcess( unsigned x, unsigned y );

    ///	N° de tapis
    unsigned char	_num;
  };

}

#endif

