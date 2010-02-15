/***************************************************************************
                          diamMonster.h  -  description
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


#ifndef ROLL_GAME_DIaMMONSTER_H
#define ROLL_GAME_DIAMMONSTER_H


#include <roll/game/hungryMonster.h>


namespace roll
{

  /**	Monstre diamivore (qui bouffe les diamants, le vilain!)  */
  class RDiamMonster : public RHungryMonster
  {
  public:
    ///
    RDiamMonster( unsigned num = 129 ) : RHungryMonster( num ) {}
    ///
    virtual ~RDiamMonster();

    ///
    virtual bool possEat( unsigned x, unsigned y );
    ///	pas écrasé par diams
    virtual bool squashedBy( const GElem* el ) const 
    { return( el->canFall() && !el->isDiam() ); }
    ///
    virtual bool explodesOnBlob() const { return( true ); }

  protected:
    ///
    virtual void realProcess( unsigned x, unsigned y );
  };

}

#endif


