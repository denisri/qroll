/***************************************************************************
                          hungryMonster.h  -  description
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


#ifndef ROLL_GAME_HUNGRYMONSTER_H
#define ROLL_GAME_HUNGRYMONSTER_H


#include <roll/game/rightMonster.h>


namespace roll
{

  /**	Monstre glouton mange-tout */
  class RHungryMonster : public RRightMonster
  {
  public:
    /**@name	Construction - destruction */
    //@{
    ///
    RHungryMonster( unsigned num = 9 );
    ///
    virtual ~RHungryMonster();
    //@}

    /**	Teste la possibilité de bouffer qqch de consistant dans la case 
	donnée. \\
	Si c'est possible, effectue son nettoyage ("bouffe").
	@param	x	coordonnée x de la case où on veut aller
	@param	y	coordonnée y de la case où on veut aller
    */
    virtual bool possEat( unsigned x, unsigned y );
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEats() const
    { return( RRSoundProcessor::HUNGRY ); }
    ///
    virtual bool explodesOnBlob() const { return( false ); }

  protected:
    ///	Mouvement directionnel
    virtual void moveDir( unsigned x, unsigned y, Direction dir );
    ///	Teste la direction
    virtual bool moveEatLeft( unsigned x, unsigned y );
    ///
    virtual bool moveEatRight( unsigned x, unsigned y );
    ///
    virtual bool moveEatUp( unsigned x, unsigned y );
    ///
    virtual bool moveEatDown( unsigned x, unsigned y );
  };

}

#endif


