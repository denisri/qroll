/***************************************************************************
                          ball.h  -  description
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


#ifndef ROLL_GAME_BALL_H
#define ROLL_GAME_BALL_H


#include <roll/game/eatable.h>
#include <roll/game/constantes.h>


namespace roll
{

  class RBall : public REatable
  {
  public:
    RBall( unsigned num, unsigned flags = BA, bool moving = false ) 
      : REatable( num ), _moving( moving ) { f = flags; }
    virtual ~RBall();

    virtual unsigned short sprite( RBack* back = 0 ) const;
    virtual unsigned short backStillSprite( RBack* back = 0 ) const;
    virtual unsigned short movingSprite() const;
    virtual bool isBall() const { return( true ); }
    virtual bool canMove() const { return( true ); }
    virtual bool isSlippery() const { return( true ); }

    ///	Donne la cause de la réflexion en (x, y)
    unsigned reflectCause( unsigned x, unsigned y );
    ///
    void setState( bool moving ) { _moving = moving; }

  protected:
    virtual void realProcess( unsigned x, unsigned y );
    bool	_moving;

  private:

  };

}


#endif


