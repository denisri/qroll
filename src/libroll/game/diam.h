/***************************************************************************
                          diam.h  -  description
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


#ifndef ROLL_GAME_DIAM_H
#define ROLL_GAME_DIAM_H


#include <roll/game/fallingEatable.h>


namespace roll
{

  /**	Diam (c'est le but du jeu) */
  class RDiam : public RFallingEatable
  {
  public:
    ///
    RDiam( unsigned num = 37 ) : RFallingEatable( num ) {}
    ///
    virtual ~RDiam();

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isDiam() const { return( true ); }
    ///
    virtual GElem* transformed() const;
    ///
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::DIAM ); }
    //@}

    /**@name	Propriétés propres à RDiam */
    //@{
    ///	Numéro de type de diam
    virtual unsigned diamNum() const;
    //@}
  };

}

#endif


