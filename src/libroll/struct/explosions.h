/***************************************************************************
                          explosions.h  -  description
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


#ifndef ROLL_STRUCT_EXPLOSIONS_H
#define ROLL_STRUCT_EXPLOSIONS_H


#include <string>


/**	Tableau des explosions
 */
class RExplosions
{
public:
  RExplosions() {}
  ~RExplosions() {}
  RExplosions & operator = ( RExplosions & exp );

  void load( const std::string & nom );

  unsigned short	a[512];
  unsigned short	d[100][3][3];
};




#endif


