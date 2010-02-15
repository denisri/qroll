/***************************************************************************
                          detonator.h  -  description
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


#ifndef ROLL_GAME_DETONATOR_H
#define ROLL_GAME_DETONATOR_H


#include <roll/game/button.h>


namespace roll
{
  /**	Détonateur pour dynamite
   */
  class RDetonator : public RButton
  {
  public:
    RDetonator( unsigned short s ) : RButton( s ), _pressed( 0 ) {}
    virtual ~RDetonator();
    virtual void activate( unsigned x, unsigned y );
    virtual void realProcess( unsigned x, unsigned y );

    virtual unsigned short sprite( RBack* b = 0 ) const;
    virtual unsigned short backStillSprite( RBack* b = 0 ) const;

  protected:
    unsigned	_pressed;
  };

}

#endif


