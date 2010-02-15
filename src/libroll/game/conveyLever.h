/***************************************************************************
                          conveyLever.h  -  description
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


#ifndef ROLL_GAME_CONVEYLEVER_H
#define ROLL_GAME_CONVEYLEVER_H


#include <roll/game/button.h>


namespace roll
{
  /**	Levier de tapis roulant
   */
  class RConveyLever : public RLever
  {
  public:
    RConveyLever( unsigned short s, unsigned num ) 
      : RLever( s, (int) num ) {}
    virtual ~RConveyLever();

    virtual void setState( int state );
    virtual int state() const;
    virtual void activate( unsigned x, unsigned y );
    virtual unsigned num() { return( (unsigned) _state ); }

    virtual unsigned short sprite( RBack* back = 0 ) const;
    virtual unsigned short backStillSprite( RBack* back = 0 ) const;

  protected:

  private:
  };

}

#endif


