/***************************************************************************
                          key.h  -  description
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


#ifndef ROLL_GAME_KEY_H
#define ROLL_GAME_KEY_H


#include <roll/game/fallingEatable.h>


namespace roll
{

  /**	Key */
  class RKey : public RFallingEatable
  {
  public:
    RKey( unsigned num ) : RFallingEatable( num ) {}
    virtual ~RKey();

    virtual bool isKey() const { return( true ); }
    ///	crée un nouvel élément transformé (si transformable)
    virtual GElem* transformed() const;
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::KEY ); }
    virtual RollSoundBank::SNDLIST soundWhenFalls() const
    { return( RollSoundBank::KEY_FALL ); }

    ///	key type number
    virtual unsigned keyNum() const;
    /// key type number -> element id (sprite number)
    static unsigned keyOfNum( unsigned num );
  };

}

#endif

