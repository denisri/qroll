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

  /**	Clef */
  class RKey : public RFallingEatable
  {
  public:
    ///
    RKey( unsigned num ) : RFallingEatable( num ) {}
    ///
    virtual ~RKey();

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isKey() const { return( true ); }
    ///	crée un nouvel élément transformé (si transformable)
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::KEY ); }
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::KEY_FALL ); }
    //@}

    /**@name	Propriétés propres à RKey */
    //@{
    ///	Numéro de type de clef
    virtual unsigned keyNum() const;
    //@}
  };

}

#endif

