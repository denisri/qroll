/***************************************************************************
                          leftMonster.h  -  description
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


#ifndef ROLL_GAME_LEFTMONSTER_H
#define ROLL_GAME_LEFTMONSTER_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Monstre (général; celui de base tourne à gauche) */
  class RLeftMonster : public GElem
  {
  public:
    RLeftMonster( unsigned num = 0 );
    virtual ~RLeftMonster();

    ///	Direction dans laquelle tourne le monstre
    enum Direction
      {
	LEFT,
	RIGHT,
	///	Normalement, celui-ci ne sert pas
	OTHER
      };

    /**	Teste la possibilité d'aller dans la case donnée. 
	Si c'est possible, effectue son nettoyage ("bouffe").
	@param	x	coordonnée x de la case où on veut aller
	@param	y	coordonnée y de la case où on veut aller
	@param	pouss	précise si on a le droit de pousser l'objet et dans 
	quel sens (-1: gauche, 0: pousse pas, 1: droite)
    */
    virtual bool poss( unsigned x, unsigned y, int pouss = 0 );

    /**@name	Propriétés de la classe */
    //@{
    virtual bool isAnimated() const { return( true ); }
    virtual AnimType animType() const { return( AnimStraight ); }
    virtual bool canMove() const { return( true ); }
    virtual bool squashedBy( const GElem* el ) const 
    { return( el->canFall() ); }
    virtual bool isExplosive() const { return( true ); }
    virtual bool explodesOnBlob() const { return( true ); }
    virtual bool isMonster() const { return( true ); }
    //@}

  protected:
    virtual void realProcess( unsigned x, unsigned y );
    ///	Appelle moveDir avec la bonne direction (pour RLeftMonster: LEFT)
    virtual void move( unsigned x, unsigned y );
    ///	Mouvement directionnel
    virtual void moveDir( unsigned x, unsigned y, Direction dir );
    ///	Teste la direction
    virtual bool moveLeft( unsigned x, unsigned y );
    virtual bool moveRight( unsigned x, unsigned y );
    virtual bool moveUp( unsigned x, unsigned y );
    virtual bool moveDown( unsigned x, unsigned y );
  };


  //	inline


  inline void RLeftMonster::move( unsigned x, unsigned y )
  {
    moveDir( x, y, LEFT );
  }

}

#endif


