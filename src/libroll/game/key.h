/***************************************************************************
                          key.h  -  description
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


#ifndef ROLL_GAME_KEY_H
#define ROLL_GAME_KEY_H


#include <roll/game/fallingEatable.h>


namespace roll
{

  /**	Clef */
  class RKey : public RFallingEatable
  {
  public:
    ///
    RKey( unsigned num ) : RFallingEatable( num ) {}
    ///
    virtual ~RKey();

    /**@name	Propri�t�s de la classe */
    //@{
    ///
    virtual bool isKey() const { return( true ); }
    ///	cr�e un nouvel �l�ment transform� (si transformable)
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::KEY ); }
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::KEY_FALL ); }
    //@}

    /**@name	Propri�t�s propres � RKey */
    //@{
    ///	Num�ro de type de clef
    virtual unsigned keyNum() const;
    //@}
  };

}

#endif

