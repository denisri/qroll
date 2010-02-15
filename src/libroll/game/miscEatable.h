/***************************************************************************
                          miscEatable.h  -  description
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


#ifndef ROLL_GAME_MISCEATABLE_H
#define ROLL_GAME_MISCEATABLE_H


#include <roll/game/fallingEatable.h>


namespace roll
{

  /**	Bonus Life (qui donne une vie de +) */
  class RBonusLife : public RFallingEatable
  {
  public:
    ///
    RBonusLife( unsigned num = 230 ) : RFallingEatable( num ) {}
    ///
    virtual ~RBonusLife() {};

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isBonusLife() const { return( true ); }
    ///
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::BONUSLIFE ); }
    //@}

  };


  /**	Pastille de force (qui rend beau et fort) */
  class RStrengthPill : public RFallingEatable
  {
  public:
    ///
    RStrengthPill( unsigned num = 233 ) : RFallingEatable( num ) {}
    ///
    virtual ~RStrengthPill() {};

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isStrengthPill() const { return( true ); }
    ///
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::STRENGTH ); }
    //@}

  };


  /**	Sablier (qui donne du temps pour finir le tableau quand on est à 
	la bourre) */
  class RHourglass : public RFallingEatable
  {
  public:
    ///
    RHourglass( unsigned num = 153 ) : RFallingEatable( num ) {}
    ///
    virtual ~RHourglass() {};

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isHourglass() const { return( true ); }
    ///
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::TIME ); }
    //@}

  };

}

#endif

