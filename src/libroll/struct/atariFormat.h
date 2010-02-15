/***************************************************************************
                          atariFormat.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivière
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


#ifndef ROLL_STRUCT_ATARIFORMAT_H
#define ROLL_STRUCT_ATARIFORMAT_H


namespace roll
{

  /** Series info for Atari format
   */
  struct AtariFormatInfo
  {
    unsigned		diams;
    unsigned		exploding;
    unsigned		blobs;
    unsigned		rating;
    unsigned short	elements_map[240];
  };

}


#endif


