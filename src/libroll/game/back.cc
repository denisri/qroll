/***************************************************************************
                          back.cc  -  description
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


#include <roll/game/back.h>
#include <roll/game/gElem.h>
#include <roll/game/vars.h>

using namespace roll;


RBack::RBack( unsigned num ) : s( num ), f( 0 )
{
}


RBack::~RBack()
{
}


unsigned short RBack::sprite() const
{
  return( s );
}


GElem* RBack::toForeground() const
{
  return( new EmptyElem );
}


// ---


RFrontBack::~RFrontBack()
{
}


// ---


MirrorBack::~MirrorBack()
{
}


GElem* MirrorBack::toForeground() const
{
  return( elFactory.createElem( s ) );
}


