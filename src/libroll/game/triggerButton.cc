/***************************************************************************
                          button.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis RiviÃ¨re
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


#include <roll/game/triggerButton.h>
#include <roll/game/vars.h>

using namespace roll;
using namespace std;


RTriggerButton::~RTriggerButton()
{
  deactivate();
}


void RTriggerButton::activate( unsigned, unsigned )
{
  _active = true;
  activeElements().insert( coverElement() );
  // TODO: sound
}


std::set<unsigned> & RTriggerButton::activeElements()
{
  static set<unsigned> activeelem;
  return activeelem;
}


bool RTriggerButton::isTriggerActive( unsigned element )
{
  return activeElements().find( element ) != activeElements().end();
}


unsigned RTriggerButton::coverElement() const
{
  if( ( s & 0xff ) % 40 < 10 )
    return s-1;
  else
    return s+1;
}


void RTriggerButton::deactivate()
{
  if( _active )
  {
    _active = false;
    unsigned elem = coverElement();
    set<unsigned> & active = activeElements();
    set<unsigned>::iterator i = active.find( elem );
    if( i != active.end() )
      active.erase( i );
  }
}


void RTriggerButton::realProccess( unsigned, unsigned )
{
  deactivate();
}


