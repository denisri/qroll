/***************************************************************************
                          diamMonster.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivière
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


#include <roll/game/diamMonster.h>
#include <roll/game/vars.h>

using namespace roll;


RDiamMonster::~RDiamMonster()
{
}


bool RDiamMonster::possEat( unsigned x, unsigned y )
{
  GElem		*&el = game.tbct.d[x][y];
  
  if( (sp_flg[el->s].l2 & INDES) || (el->f & INDE) || el->isEmpty() )
    return( false );

  if( el->isDiam() || el->s == 230 )	// (mange aussi les bonus life)
    return( true );
  return( false );
}


void RDiamMonster::realProcess( unsigned x, unsigned y )
{
  if( ( f & RETPET ) && game.tbct.d[x][y-1]->isDiam() )
    f &= ~RETPET;	//	pour immuniser aux chutes de diams

  RHungryMonster::realProcess( x, y );
}


