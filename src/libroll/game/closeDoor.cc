/***************************************************************************
                          closeDoor.cc  -  description
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


#include <roll/game/closeDoor.h>
#include <roll/game/door.h>
#include <roll/game/vars.h>
#include <roll/game/back.h>

using namespace roll;


RCloseDoor::RCloseDoor( unsigned num ) : GElem( num )
{
}


RCloseDoor::~RCloseDoor()
{
}


void RCloseDoor::realProcess( unsigned x, unsigned y )
{
  if( game.tbct.doorOpen )
    {
      unsigned	flgs = f;
      game.tbct.d[x][y] = new OpenDoor( s+1 );
      game.tbct.d[x][y]->f = flgs;	// copy flags
      flgs = game.tbct.b[x][y]->f;
      delete game.tbct.b[x][y];
      // move to background as well
      game.tbct.b[x][y] = new MirrorBack( s+1 );
      game.tbct.b[x][y]->f = flgs;
      RRSoundProcessor::processor().process( RRSoundProcessor::DOOR );
      delete this;	// oui; je sais... mais comment je fais, sinon ?
    }
}




