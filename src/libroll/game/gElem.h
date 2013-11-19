/***************************************************************************
                          gElem.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivière
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


#ifndef QROLL_GAME_GELEM_H
#define QROLL_GAME_GELEM_H


#include <roll/game/back.h>
#include <roll/game/constantes.h>
#include <roll/sound/rollsoundbank.h>

#include <stdlib.h>


namespace roll
{

  /**	Game Element. \\
	Classe de base pour les différents objets: monstres, mec, rocks, 
	herbe, etc.
  */
  class GElem
  {
  public:
    /**@name	Types */
    //@{
    ///
    enum AnimType
      {
	///
	AnimNone, 
	///	123123
	AnimStraight, 
	///	12321
	AnimReturn, 
	///	vertical
	AnimVertical
      };
    //@}

    /**@name	Constructeur - destructeur */
    //@{
    ///	Constructeur pas encore protégé pour l'instant
    GElem( unsigned num ) : s( num ), f( rand() & ANIMA ) {}
    ///
    virtual ~GElem();
    //@}

    /**@name	Fonctions */
    //@{
    ///	Sprite à afficher (quand seul)
    virtual unsigned short sprite( RBack* back = 0 ) const;
    ///	Sprite qui bouge (ou vide s'il n'y en a pas)
    virtual unsigned short movingSprite() const;
    ///	Sprite de fond (immobile, derrière celui qui bouge)
    virtual unsigned short backStillSprite( RBack* back = 0 ) const;
    ///	Sprite immobile de 1er plan: devant celui qui bouge
    virtual unsigned short frontStillSprite( RBack* back = 0 ) const;
    ///	Gestion pendant le jeu (mouvements, tranformations etc)
    virtual void process( unsigned x, unsigned y );
    /**	Fait exploser le sprite (si explosif).
	! Peut détruire l'élément appelant ! (fonction statique) */
    static void explode( unsigned x, unsigned y );
    virtual bool isAnimated() const;
    ///	Calcule le sprite d'animation
    virtual unsigned short animation() const;
    ///	Calcule le sprite animé du background
    virtual unsigned short backAnim( RBack* back ) const;
    /** ID identifying this particular element - used for packing / saving
	/ transmitting levels by network from elements numbers */
    virtual unsigned short idNumber() const { return( s ); }
    //@}

    /**@name	Propriétés de la classe */
    //@{
    virtual bool isEmpty() const { return( false ); }
    virtual bool isKey() const { return( false ); }
    virtual bool isLock() const { return( false ); }
    virtual bool isDiam() const { return( false ); }
    virtual bool isRock() const { return( false ); }
    virtual bool isDoor() const { return( false ); }
    virtual bool isRoll() const { return( false ); }
    virtual bool isBomb() const { return( false ); }
    virtual bool isGrass() const { return( false ); }
    virtual bool isJungle() const { return( false ); }
    virtual bool isBonusLife() const { return( false ); }
    virtual bool isStrengthPill() const { return( false ); }
    virtual bool isHourglass() const { return( false ); }
    virtual bool isBall() const { return( false ); }
    virtual bool isBlob() const { return( false ); }
    virtual bool isMonster() const { return( false ); }
    virtual bool isButton() const { return( false ); }
    virtual bool isLever() const { return( false ); }
    virtual AnimType animType() const;
    virtual bool canFall() const { return( false ); }
    virtual bool canMove() const { return( false ); }
    ///	se mange (par Roll)
    virtual bool isEatable() const { return( false ); }
    ///	est écrasable par el ?
    virtual bool squashedBy( const GElem* el ) const;
    ///	crée un nouvel élément transformé (si transformable)
    virtual GElem* transformed() const;
    ///	Explosif d'une manière ou d'une autre ?
    virtual bool isExplosive() const { return( false ); }
    ///	Explosif quand tombe ?
    virtual bool explodesWhenFalls() const { return( false ); }
    ///	Explosif quand dans le rayon d'une explosion ?
    virtual bool explodesWhenBlown() const { return( false ); }
    ///	Explosif quand créé par une explosion ?
    virtual bool chainExplodes() const { return( false ); }
    ///	Explosif au contact de la bébette
    virtual bool explodesOnBlob() const { return( false ); }
    ///	L'élément est-il là temporairement ?
    virtual bool isTemporary() const { return( false ); }
    virtual bool isSlippery() const;
    /**	Not all elements can be placed in the limits of the level, esp. not 
	moving elements which could go outside of it or leave a hole there: 
	borders must be 'solid' elements */
    virtual bool canBeInBorder() const;
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::NO_SOUND ); }
    virtual RollSoundBank::SNDLIST soundWhenFalls() const
    { return( RollSoundBank::NO_SOUND ); }
    //@}

    /**@name	Instance properties */
    //@{
    bool isBusy() const { return( f & BUSY ); }
    virtual bool isMoving() const { return( ( f & (GAU|DROI|HAU|BA) ) != 0 ); }
    virtual bool isFalling() const { return( ( f & BA ) != 0 ); }
    virtual void setFalling( bool x ) { if( x ) f |= BA; else f &= ~BA; }
    virtual bool isWaiting() const { return( ( f & WAIT ) != 0 ); }
    virtual void setWaiting( bool state )
    { f = (f & ~(WAIT | REMAIN)) | (state ? WAIT : 0); }
    virtual bool hasExtFlg() const { return( ( f & EXTFLG ) != 0 ); }
    virtual void setExtFlg( bool state )
    { f = (f & ~(EXTFLG | REMAIN)) | (state ? EXTFLG : 0); }
    virtual bool aboutToExplode() const
    { return( ( f & ( PETE | RETPET ) ) != 0 ); }
    //@}

    /**@name	Data */
    //@{
    ///	Sprite number with page
    unsigned short s;
    ///	Flags
    unsigned long  f;
    //@}

  protected:
    virtual void realProcess( unsigned x, unsigned y );

  private:
  };


  ///	Elément vide
  class EmptyElem : public GElem
  {
  public:
    ///
    EmptyElem() : GElem( 255 ) {}
    ///
    virtual ~EmptyElem() {}

    ///
    virtual bool isEmpty() const { return( true ); }
    ///
    virtual bool isEatable() const { return( true ); }
    ///
    virtual unsigned short sprite( RBack* back ) const;
    ///
    virtual unsigned short backStillSprite( RBack* back ) const;

  protected:
  };


  ///	Herbe
  class RGrass : public GElem
  {
  public:
    ///
    RGrass( unsigned num = 19 ) : GElem( num ) {}
    ///
    virtual ~RGrass() {}

    ///
    virtual bool isGrass() const { return( true ); }
    ///
    virtual bool isEatable() const { return( true ); }
    ///
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::GRASS ); }

  protected:
  };


  ///	Jungle
  class RJungle : public GElem
  {
  public:
    ///
    RJungle( unsigned num = 30 ) : GElem( num ) {}
    ///
    virtual ~RJungle() {}

    ///
    virtual bool isJungle() const { return( true ); }
    ///
    virtual bool isEatable() const { return( true ); }
    ///
    virtual RollSoundBank::SNDLIST soundWhenEaten() const
    { return( RollSoundBank::GRASS ); }

  protected:
  };


  inline bool GElem::squashedBy( const GElem* ) const { return( false ); }

}


#endif


