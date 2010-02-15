/***************************************************************************
                          detonator.cc  -  description
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


#include <roll/game/detonator.h>
#include <roll/game/vars.h>

using namespace roll;


RDetonator::~RDetonator()
{
}


void RDetonator::activate( unsigned, unsigned )
{
  _pressed = 2;
}


unsigned short RDetonator::sprite( RBack* ) const
{
  if( _pressed )
    return( s + 40 );
  return( s );
}


unsigned short RDetonator::backStillSprite( RBack* ) const
{
  if( _pressed )
    return( s + 40 );
  return( s );
}


void RDetonator::realProcess( unsigned, unsigned )
{
  if( _pressed )
    {
      --_pressed;
      if( !_pressed )
	{
	  game.tbct.explodeDyna = 2;
	}
    }
}



