/***************************************************************************
                          bomb.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivi�re
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

  /**	Bombe �teinte */
  class RBomb : public RFallingEatable
  {
  public:
    ///
    RBomb( unsigned num = 33 ) 
      : RFallingEatable( num ) {}
    ///
    virtual ~RBomb() {}

    /**@name	Propri�t�s de la classe */
    //@{
    ///
    virtual bool isBomb() const { return( true ); }
    ///	cr�e un nouvel �l�ment transform� (si transformable)
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::BOMB ); }
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::BOMB ); }
    //@}

    /**@name	Propri�t�s propres � Rbomb */
    //@{
    ///	Num�ro de type de bombe
    virtual unsigned bombNum() const;
    ///	Donne le no de la bombe allum�e correspondant au type voulu
    static unsigned short burningBomb( unsigned num );
    //@}

  protected:
  };

}

#endif


