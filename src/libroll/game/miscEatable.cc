/***************************************************************************
                          miscEatable.cc  -  description
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


#include <roll/game/miscEatable.h>
#include <roll/game/vars.h>

using namespace roll;


GElem* RBonusLife::transformed() const
{
  return( elFactory.createElem( s-40 ) );
}


GElem* RStrengthPill::transformed() const
{
  return( elFactory.createElem( s-80 ) );
}


GElem* RHourglass::transformed() const
{
  return( elFactory.createElem( s+40 ) );
}



