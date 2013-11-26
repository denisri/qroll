/***************************************************************************
                          conveyorBelt.cc  -  description
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


#include <roll/game/conveyorBelt.h>
#include <roll/game/vars.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;


void RConveyorBelt::realProcess( unsigned x, unsigned y )
{
  // direction of this kind of conveyor
  int	dir = game.tbct.conveyDir[ _num ];
  unsigned	flags = 0;

  switch( dir )
  {
  case -1:	// left
    flags = GAU;
  case 1:	// right
    {
      RRSoundProcessor::processor().processIfNotUsed
        ( RollSoundBank::CONVEYORBELT );
      if( !flags ) flags = DROI;
      GElem	*&next = game.tbct.d[x+dir][y-1];
      GElem	*&over = game.tbct.d[x][y-1];
      if( next->isEmpty() && over->canMove() && !over->isBusy() )
      {
        if( over->hasExtFlg() )
        {
          delete next;
          next = over;
          over->f |= ( f & ~(GAU | DROI | HAU | BA) ) | flags;
          over = new EmptyElem;
          if( dir == -1 ) next->setExtFlg( true );
          else next->setExtFlg( false );
        }
        else over->setExtFlg( true );
      }
    }
    break;
  default:	// stopped
    break;
  }
}


unsigned short RConveyorBelt::animation() const
{
  const unsigned short basesprite = 0x1a0;
  switch( game.tbct.conveyDir[_num] )
  {
  case 1:
    return( basesprite + 2 - (anim % 3) );
  case -1:
    return( basesprite + (anim % 3) );
  default:
    return( basesprite );
  }
}



