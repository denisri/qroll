/***************************************************************************
                          portcullis.cc  -  description
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


#include <roll/game/portcullis.h>
#include <roll/game/openPortcullis.h>
#include <roll/game/vars.h>

using namespace roll;


RPortcullis::~RPortcullis()
{
}


unsigned short RPortcullis::sprite( RBack* back ) const
{
  unsigned short	spr = REatable::sprite( back );
  if( spr != s )
    return( spr );

  if( _state == CLOSED )
    return( s );
  else
    return( s - 40 );
}


unsigned short RPortcullis::backStillSprite( RBack* back ) const
{
  unsigned short	spr = REatable::backStillSprite( back );
  if( spr != s )
    return( spr );

  if( _state == CLOSED )
    return( s );
  else
    return( s - 40 );
}


unsigned short RPortcullis::frontStillSprite( RBack* back ) const
{
  unsigned short	spr = REatable::frontStillSprite( back );
  if( spr != s )
    return( spr );

  if( _state == CLOSED )
    return( s );
  else
    return( s - 40 );
}


void RPortcullis::realProcess( unsigned x, unsigned y )
{
  switch( _state )
    {
    case OPENING:
      //cout << "herse, ouverture ( " << s << " )\n";
      game.tbct.d[x][y] = new EmptyElem;
      delete game.tbct.b[x][y];
      game.tbct.b[x][y] = new ROpenPortcullis( s - 80 );
      delete this;
      break;
    case CLOSING:
      //cout << "herse, fermeture ( " << s << " )\n";
      _state = CLOSED;
      break;
    default:
      break;
    }
}


