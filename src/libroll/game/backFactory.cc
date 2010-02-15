/***************************************************************************
                          backFactory.cc  -  description
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


#include <roll/game/backFactory.h>
#include <roll/game/back.h>
#include <roll/game/openPortcullis.h>

using namespace roll;


RBackFactory::RBackFactory()
{
}


RBackFactory::~RBackFactory()
{
}


RBack* RBackFactory::createBack( unsigned num )
{
  switch( num )
    {
    case 29:		// opened door
    case 69:
    case 109:
    case 149:
    case 189:
    case 229:
      return( new MirrorBack( num ) );
    case 226:		// illusion
      return( new RFrontBack( 20 ) );
    case 0x100+186:	// diaphragme
      return( new RBack( num ) );
    case 0x100+149:	// herse
      return( new ROpenPortcullis( num ) );
    default:
      return( new RBack( 255 ) );
    }
}





