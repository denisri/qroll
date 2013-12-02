/***************************************************************************
                          rock.cc  -  description
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


#include <roll/game/breakablerock.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <roll/sound/soundProcessor.h>


using namespace roll;


RBreakableRock::RBreakableRock( unsigned num ) : Rock( num )
{
}


RBreakableRock::~RBreakableRock()
{
}


void RBreakableRock::doExplode( unsigned x, unsigned y )
{
  unsigned long         fl;

  if( x == 0 || y == 0 || x+1 == game.tbct.sizeX() 
      || y+1 == game.tbct.sizeY() )
    return;   // in border, do not explode

  RBack *&bk = game.tbct.b[x][y];
  if( !(sp_flg[bk->s].l2 & BC_INDES) && !(bk->f & BI_INDE) )
    // background destructible
  {
    unsigned short next = nextBreakingElement();
    if( next != 0xffff )
    {
      progressBreak();
    }
    else
    {
      //      change background
      delete bk;
      bk = backFactory.createBack( explo.d[explo.a[s]][x % 3][y % 3] );

      GElem *& ge = game.tbct.d[x][y]; // location of this

      if( !(sp_flg[s].l2 & INDES) && !(f & INDE) ) // 1er plan
      {
//         fl = EXP1;
        ge = elFactory.createElem( explo.d[explo.a[s]][x % 3][y % 3] );
        ge->f |= fl;
        if( ge->chainExplodes() )
          ge->f |= PETE;

        // TODO: hatch sound
        // RRSoundProcessor::processor().process( RollSoundBank::EXPLO );
        delete this;
      }
    }
  }
}


unsigned short RBreakableRock::nextBreakingElement() const
{
  unsigned short x = ( s & 0xff ) / 40;
  if( x != 3 && x != 5 )
    return s + 40;
  return 0xffff;
}


void RBreakableRock::progressBreak()
{
  unsigned short x = ( s & 0xff ) / 40;
  if( x != 3 && x != 5 )
    s += 40;
}


