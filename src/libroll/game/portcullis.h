/***************************************************************************
                          portcullis.h  -  description
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


#ifndef ROLL_GAME_PORTCULLIS_H
#define ROLL_GAME_PORTCULLIS_H


#include <roll/game/eatable.h>

namespace roll
{

  class RPortcullis : public REatable
  {
  public:
    enum State
      {
	CLOSED, 
	OPENING, 
	CLOSING
      };

    RPortcullis( unsigned num, State state = CLOSED ) 
      : REatable( num ), _state( state ) {}
    virtual ~RPortcullis();
    State state() const { return( _state ); }
    void setState( State state ) { _state = state; }
    virtual unsigned short sprite( RBack* back = 0 ) const;
    virtual unsigned short backStillSprite( RBack* back = 0 ) const;
    virtual unsigned short frontStillSprite( RBack* back = 0 ) const;

  protected:
    virtual void realProcess( unsigned x, unsigned y );

    State	_state;
  };

}


#endif


