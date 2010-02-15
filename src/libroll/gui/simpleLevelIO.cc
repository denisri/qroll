/***************************************************************************
                          simpleLevelIO.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2004 by Denis Rivière
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

#ifdef _WIN32
#define OStream QDataStream
#define IStream QDataStream
#endif
#include <roll/gui/qdatastreamWrapper.h>


using namespace roll;
using namespace std;

// compile IO for QDataStream
template void SimpleLevel::write( QDataStream & ) const;
template bool SimpleLevel::read( QDataStream &, unsigned );


