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


#ifndef ROLL_GAME_TRIGGERBUTTON_H
#define ROLL_GAME_TRIGGERBUTTON_H


#include <roll/game/button.h>
#include <set>

namespace roll
{

  class RTriggerButton : public RButton
  {
  public:
    RTriggerButton( unsigned short s ) : RButton( s ), _active( false ) {}
    virtual ~RTriggerButton();

    virtual void activate( unsigned x, unsigned y );
    virtual unsigned coverElement() const;

    static bool isTriggerActive( unsigned element );

  protected:
    virtual void realProccess( unsigned x, unsigned y );
    static std::set<unsigned> & activeElements();
    virtual void deactivate();

  private:
    bool _active;
  };

}

#endif

