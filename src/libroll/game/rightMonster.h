/***************************************************************************
                          rightMonster.h  -  description
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


#ifndef ROLL_GAME_RIGHTMONSTER_H
#define ROLL_GAME_RIGHTMONSTER_H


#include <roll/game/leftMonster.h>


namespace roll
{

  /**	Monstre qui tourne à droite */
  class RRightMonster : public RLeftMonster
  {
  public:
    ///
    RRightMonster( unsigned num = 0 );
    ///
    virtual ~RRightMonster();

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual AnimType animType() const { return( AnimReturn ); }
    //@}

  protected:
    ///
    virtual void move( unsigned x, unsigned y );
  };


  //	inline


  inline void RRightMonster::move( unsigned x, unsigned y )
  {
    moveDir( x, y, RIGHT );
  }

}

#endif


