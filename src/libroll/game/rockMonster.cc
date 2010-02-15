/***************************************************************************
                          rockMonster.cc  -  description
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


#include <roll/game/rockMonster.h>
#include <roll/game/vars.h>

using namespace roll;


RRockMonster::~RRockMonster()
{
}


bool RRockMonster::possEat( unsigned x, unsigned y )
{
  GElem		*&el = game.tbct.d[x][y];
  
  if( (sp_flg[el->s].l2 & INDES) || (el->f & INDE) || el->isEmpty() )
    return( false );

  if( el->isRock() )
    return( true );

  return( false );
}


