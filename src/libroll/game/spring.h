/***************************************************************************
                          spring.h  -  description
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


#ifndef ROLL_GAME_SPRING_H
#define ROLL_GAME_SPRING_H


#include <roll/game/gElem.h>


namespace roll
{

  ///	Ressort qui se détend
  class RSpring : public GElem
  {
  public:
    enum Direction
      {
	HORIZONTAL, 
	VERTICAL
      };

    enum State
      {
	WAITING, 
	EXTENDED_UP, 
	EXTENDED_DOWN, 
	EXTENDED_LEFT, 
	EXTENDED_RIGHT, 
	UP, 
	DOWN, 
	LEFT, 
	RIGHT
      };

    RSpring( unsigned num, Direction dir = HORIZONTAL, State st = WAITING ) 
      : GElem( num ), direction( dir ), state( st ) {}
    virtual ~RSpring() {}

    State dir( int dx, int dy ) const;
    State extendDir( int dx, int dy ) const;
    virtual unsigned short sprite( RBack* back = 0 ) const;
    virtual unsigned short backStillSprite( RBack* back ) const;
    virtual bool isTemporary() const
    { switch( state )
      { case UP: case DOWN: case LEFT: case RIGHT: return( true );
      default: return( false ); } }
    virtual bool canBeInBorder() const { return( true ); }

  protected:
    virtual void realProcess( unsigned x, unsigned y );
    virtual void checkExtend( unsigned x, unsigned y );
    virtual bool checkExtendDir( unsigned x, unsigned y, int dx, int dy );

    Direction	direction;
    State		state;

  private:
  };

}

#endif

