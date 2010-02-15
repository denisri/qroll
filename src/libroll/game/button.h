/***************************************************************************
                          button.h  -  description
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


#ifndef ROLL_GAME_BUTTON_H
#define ROLL_GAME_BUTTON_H


#include <roll/game/eatable.h>


namespace roll
{
  /**	Boutons poussoirs, classe de base abstraite
   */
  class RButton : public REatable
  {
  public:
    RButton( unsigned short s ) : REatable( s ) {}
    virtual ~RButton();

    virtual bool isButton() const { return( true ); }
    virtual void activate( unsigned x, unsigned y ) = 0;

  protected:

  private:
  };



  /**	Levier, classe de base abstraite
   */
  class RLever : public RButton
  {
  public:
    RLever( unsigned short s, int state = 0 ) : RButton( s ), _state( state ) {}
    virtual ~RLever();

    virtual bool isLever() const {return( true ); }
    virtual int state() const { return( _state ); }
    virtual void setState( int state ) { _state = state; }

  protected:
    int	_state;

  private:
  };

}

#endif

