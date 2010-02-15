/***************************************************************************
                          blobdescr.cc  -  description
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


#include <roll/struct/blobDescr.h>

using namespace roll;


BlobDescr & BlobDescr::operator = ( BlobDescr & beb )
{
  int	i, j;

  tmax = beb.tmax;
  cpt = beb.cpt;
  flg = beb.flg;
  blk = beb.blk;
  exi = beb.exi;
  for( i=0; i<3; i++ ) for( j=0; j<3; j++ )
    {
      d1[i][j] = beb.d1[i][j];
      d2[i][j] = beb.d2[i][j];
    }

  return( *this );
}








