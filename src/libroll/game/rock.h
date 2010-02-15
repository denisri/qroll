/***************************************************************************
                          rock.h  -  description
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


#ifndef ROLL_GAME_ROCK_H
#define ROLL_GAME_ROCK_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Truc qui tombe */
  class RFallingElem : public GElem
  {
  public:
    RFallingElem( unsigned num );
    virtual ~RFallingElem();

    ///	Mouvement tombant
    virtual void moveDown( unsigned x, unsigned y );
    ///	Mouvement roulant
    virtual void moveLeft( unsigned x, unsigned y );
    ///	Mouvement roulant
    virtual void moveRight( unsigned x, unsigned y );

    virtual bool canFall() const { return( true ); }
    virtual bool canMove() const { return( true ); }
    virtual bool isTemporary() const { return( ( f & BOUGE ) != 0 ); }
    ///	en général, c'est glissant, un truc qui tombe
    virtual bool isSlippery() const { return( true ); }
    virtual bool canEat( const GElem* el ) const { return( el->isEmpty() ); }
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::ROCK ); }

  protected:
    virtual void realProcess( unsigned x, unsigned y );
    virtual void sound() const;
  };


  /**	Rock (c'est le problème du jeu (parce que "Rock and Roll")) */
  class Rock : public RFallingElem
  {
  public:
    Rock( unsigned num = 22 ) : RFallingElem( num ) {}
    virtual ~Rock() {}

    virtual bool isRock() const { return( true ); }
    virtual GElem* transformed() const;

  protected:
  };


  /**	Rocher qui tourne en roulant (tête de mort...)
   */
  class RollingRock : public Rock
  {
  public:
    RollingRock( unsigned num = 39 ) : Rock( num ) {}
    virtual ~RollingRock() {}
    virtual void moveLeft( unsigned x, unsigned y );
    virtual void moveRight( unsigned x, unsigned y );
  };


  /**	Rocher nitro normal (pète quand il tombe) */
  class RNitroRock : public Rock
  {
  public:
    RNitroRock( unsigned num = 199 ) : Rock( num ) {}
    virtual ~RNitroRock();

    virtual bool isExplosive() const { return( true ); }
    virtual bool explodesWhenFalls() const { return( true ); }

  protected:
  };


  /**	Rocher nitro+ qui pète pour un oui ou pour un non */
  class RNitroPlusRock : public RNitroRock
  {
  public:
    RNitroPlusRock( unsigned num = 239 ) : RNitroRock( num ) {}
    virtual ~RNitroPlusRock();

    virtual bool squashedBy( const GElem* el ) const
    { return( el->canFall() ); }
    virtual bool explodesWhenBlown() const { return( true ); }
    virtual bool chainExplodes() const { return( true ); }
  };


  /**	Rocher qui bouffe l'herbe */
  class RGrassRock : public Rock
  {
  public:
    RGrassRock( unsigned num = 193 ) : Rock( num ) {}
    virtual ~RGrassRock() {}

    virtual GElem* transformed() const;

    virtual bool canEat( const GElem* el ) const;
  };

}

#endif


