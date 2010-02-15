/***************************************************************************
                          burningBomb.h  -  description
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


#ifndef ROLL_GAME_BURNINGBOMB_H
#define ROLL_GAME_BURNINGBOMB_H


#include <roll/game/rock.h>


namespace roll
{

  /**	Bombe allum�e */
  class RBurningBomb : public RFallingElem
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///
    RBurningBomb( unsigned num = 73, unsigned char time = 3 ) 
      : RFallingElem( num ), _timing( time ) {}
    ///
    virtual ~RBurningBomb() {}
    //@}

    /**@name	Propri�t�s de la classe */
    //@{
    ///	Calcule le sprite d'animation
    virtual unsigned short animation() const;
    ///	cr�e un nouvel �l�ment transform� (si transformable)
    virtual GElem* transformed() const;
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::BOMB ); }
    ///
    virtual bool isExplosive() const { return( true ); }
    //@}

    /**@name	Propri�t�s propres � RBurningBomb */
    //@{
    ///
    unsigned char	timing() const { return( _timing ); }
    ///
    void setTiming( unsigned char tim ) { _timing = tim; }
    //@}

  protected:
    ///
    virtual void realProcess( unsigned x, unsigned y );
    ///
    unsigned char	_timing;
  };

}

#endif


