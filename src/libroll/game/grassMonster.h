/***************************************************************************
                          grassMonster.h  -  description
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


#ifndef ROLL_GAME_GRASSMONSTER_H
#define ROLL_GAME_GRASSMONSTER_H


#include <roll/game/hungryMonster.h>


namespace roll
{

  /**	Monstre herbivore (un genre de vache, quoi)  */
  class RGrassMonster : public RHungryMonster
  {
  public:
    ///
    RGrassMonster( unsigned num = 89 ) : RHungryMonster( num ) {}
    ///
    virtual ~RGrassMonster();

    ///
    virtual bool possEat( unsigned x, unsigned y );
    ///
    virtual bool explodesOnBlob() const { return( true ); }
  protected:
  };

}

#endif


