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


#include <roll/game/triggerCover.h>
#include <roll/game/triggerButton.h>
#include <roll/game/vars.h>

using namespace roll;


RTriggerCover::~RTriggerCover()
{
}


void RTriggerCover::realProcess( unsigned x, unsigned y )
{
  if( RTriggerButton::isTriggerActive( s ) )
  {
    game.tbct.d[x][y] = elFactory.createElem( explo.d[explo.a[s]][1][1] );
    delete this;
  }
}


// ---

RExplodingTriggerCover::~RExplodingTriggerCover()
{
}


void RExplodingTriggerCover::realProcess( unsigned x, unsigned y )
{
  if( RTriggerButton::isTriggerActive( s ) )
    explode( x, y );
}


