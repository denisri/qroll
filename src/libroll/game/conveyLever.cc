/***************************************************************************
                          conveyLever.cc  -  description
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


#include <roll/game/conveyLever.h>
#include <roll/game/vars.h>
#include <roll/sound/soundProcessor.h>
#include <roll/sound/rollsoundbank.h>

using namespace roll;


RConveyLever::~RConveyLever()
{
}


int RConveyLever::state() const
{
  return( game.tbct.conveyDir[ _state ] );
}


void RConveyLever::setState( int st )
{
  game.tbct.conveyDir[ _state ] = st;
}


void RConveyLever::activate( unsigned, unsigned )
{
  int	st = state();
  if( st < 2 )
    ++st;
  else
    st = -1;
  setState( st );
  RRSoundProcessor::processor().process( RollSoundBank::CONVEYLEVER );
}


unsigned short RConveyLever::sprite( RBack* ) const
{
  const unsigned short basesprite = 0x17e;
  int	st = state();
  if( st > 1 )
    return( basesprite + 40 );	// stopped
  return( basesprite + (1 - st) * 40 );
}


unsigned short RConveyLever::backStillSprite( RBack* ) const
{
  const unsigned short basesprite = 0x17e;
  int	st = state();
  if( st > 1 )
    return( basesprite + 40 );	// stopped
  return( basesprite + (1 - st) * 40 );
}



