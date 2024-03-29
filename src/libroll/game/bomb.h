/***************************************************************************
                          bomb.h  -  description
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


#ifndef ROLL_GAME_BOMB_H
#define ROLL_GAME_BOMB_H


#include <roll/game/fallingEatable.h>


namespace roll
{

  /**	Bombe éteinte */
  class RBomb : public RFallingEatable
  {
  public:
    ///
    RBomb( unsigned num = 33 ) 
      : RFallingEatable( num ) {}
    ///
    virtual ~RBomb() {}

    ///
    virtual bool isBomb() const { return( true ); }
    ///	crée un nouvel élément transformé (si transformable)
    virtual GElem* transformed() const;
    ///
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::BOMB ); }
    ///
    virtual RollSoundBank::SNDLIST soundWhenFalls() const
    { return( RollSoundBank::BOMB ); }
    virtual bool isExplosive() const { return true; }

    ///	Numéro de type de bombe
    virtual unsigned bombNum() const;
    ///	Donne le no de la bombe allumée correspondant au type voulu
    static unsigned short burningBomb( unsigned num );

  protected:
  };

}

#endif


