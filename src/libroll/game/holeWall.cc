/***************************************************************************
                          holeWall.cc  -  description
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


#include <roll/game/holeWall.h>
#include <roll/game/vars.h>

using namespace roll;


bool RHoleWall::testGoOut( unsigned, unsigned )
{
  delete _inside;	// le gorêt...
  _inside = 0;		// burp!...

  return( false );	// "moi? j'ai rien fait, moi..."
}




