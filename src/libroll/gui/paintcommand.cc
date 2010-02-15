/***************************************************************************
                          paintcommand.cc  -  description
                             -------------------
    begin                : 2002
    copyright            : (C) 2002 by Denis Rivière
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

#include <roll/gui/paintcommand.h>

using namespace roll;
using namespace std;

PaintCommand::PaintCommand()
{
}


PaintCommand::~PaintCommand()
{
}


void PaintCommand::size() const
{
}


// -----------------------


PaintStack::PaintStack()
{
}


PaintStack::~PaintStack()
{
}


void PaintStack::push( PaintCommand* c )
{
  _stack.push( c );
}


PaintCommand* PaintStack::pop()
{
}


PaintCommand* PaintStack::forget()
{
}


unsigned PaintStack::size() const
{
  stack<PaintCommand*>::const_iterator	i, e = _stack.end();
  unsigned				s = 0;
  for( i=0; i<e; ++i )
    s += (*i)->size();
  return( s );
}


