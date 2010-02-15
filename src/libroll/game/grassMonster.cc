/***************************************************************************
                          grassMonster.cc  -  description
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


#include <roll/game/grassMonster.h>
#include <roll/game/vars.h>
#include <roll/game/random.h>


using namespace roll;


RGrassMonster::~RGrassMonster()
{
}


bool RGrassMonster::possEat( unsigned x, unsigned y )
{
  GElem		*&el = game.tbct.d[x][y];
  unsigned	spr = el->s;
  
  if( (sp_flg[spr].l2 & INDES) || (el->f & INDE) || el->isEmpty() )
    return( false );

  if( el->isGrass() )
    return( true );
  if( el->isJungle() && (rrand() & 0xff) <= game.tbct.strength() )
    return( true );

  return( false );
}


