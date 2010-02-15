/***************************************************************************
                          blob.cc  -  description
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


#include <roll/game/blob.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


RBlob::RBlob( unsigned num ) : GElem( num )
{
}


RBlob::~RBlob()
{
}


void RBlob::realProcess( unsigned x, unsigned y )
{
  unsigned	nob = explo.a[s];	// no de bÃ©bette

  if( bebet[nob].flg == 1 )	// transformation 1
    {
      game.tbct.d[x][y] = elFactory.createElem( bebet[nob].d1[x%3][y%3] );
      delete this;		// oui, oui...
      return;
    }

  if( bebet[nob].flg == 2 )	// transformation 2
    {
      game.tbct.d[x][y] = elFactory.createElem( bebet[nob].d2[x%3][y%3] );
      delete this;		// oui, oui...
      return;
    }

  ++bebet[nob].cpt;		// compteur de bÃ©bettes
  RRSoundProcessor::processor().processIfNotUsed( RRSoundProcessor::BLOB );

  if( x == 0 ) bebet[nob].flg = 3;	// bord: pas transformable
  else grow( x-1, y, nob );

  if( x == game.tbct.sizeX()-1 )
    bebet[nob].flg = 3;
  else grow( x+1, y, nob, BUSY );

  if( y == 0 ) bebet[nob].flg = 3;
  else grow( x, y-1, nob );

  if( y == game.tbct.sizeY()-1 )
    bebet[nob].flg = 3;
  else grow( x, y+1, nob, BUSY );
}


bool RBlob::grow( unsigned x, unsigned y, unsigned nob, unsigned flags )
{
  GElem	*&el = game.tbct.d[x][y];

  if( el->explodesOnBlob() )	// explodes in contact of a blob ?
    {
      el->f |= PETE;
      bebet[nob].blk = false;	// not blocked
      return( false );
    }

  if( sp_flg[ el->s ].l2 & BEBCOM )	// mangeable par bÃ©b.?
    {
      bebet[nob].blk = false;	// pas bloquÃ©e

      if( (rrand() & 0xff) < game.tbct.speed() )  // test de grossissement
	{
	  delete el;
	  el = elFactory.createElem( s );
	  el->f |= flags;
	}
      return( true );
    }

  return( false );
}






