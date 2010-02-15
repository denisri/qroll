/***************************************************************************
                          levelAtari.cc  -  description
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


#include <roll/struct/levelAtari.h>

using namespace roll;
using namespace std;


LevelAtari::LevelAtari() 
  : Level(), LevelPar(), size( 0 ), param( 0 ), data( 0 )
{
}


LevelAtari::~LevelAtari()
{
  if( param ) delete[] param;
  param = 0;
  if( data ) delete[] data;
  data = 0;
}


void LevelAtari::load( ifstream & fich )
{
  unsigned char	c1,c2;
  unsigned long	i;

  if( param ) delete[] param;
  param = 0;

  if( data ) delete[] data;
  data = NULL;

  fich >> tx >> ty;
  fich >> c1 >> c2;
  diam = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  tps = ((unsigned short)c1<<8) + c2;
  fich >> perm >> vit >> forc >> vgen;
  fich >> c1 >> c2;
  cm = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  cd = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  cr = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  cm1 = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  ch = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  LevelPar::flags = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  tbeb = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  dbom = ((unsigned short)c1<<8) + c2;
  fich >> c1 >> c2;
  debu = ((unsigned short)c1<<8) + c2;

  sparam = debu - 28;
  param = new unsigned char[sparam];
  for( i=0; i<sparam; i++ ) fich >> param[i];

  size = debu + (unsigned long)tx*ty;

  data = new unsigned char[(unsigned long)tx*ty];

  for( i=0; i<(unsigned long)tx*ty; i++ )
    fich >> data[i];
}


void LevelAtari::resize( unsigned sx, unsigned sy )
{
  if( sx == tx && sy == ty )
    return;

  unsigned char	*olddata = data;
  data = new unsigned char[ sx * sy ];
  unsigned	x, y, nx = sx, ny = sy;

  if( tx < nx )
    nx = tx;
  if( ty < ny )
    ny = ty;

  for( y=0; y<ny; ++y )
    {
      for( x=0; x<nx; ++x )
	data[ x + sx * y ] = olddata[ x + tx * y ];
      for( ; x<sx; ++x )
	data[ x + sx * y ] = 255;
    }
  for( ; y<sy; ++y )
    for( x=0; x<sx; ++x )
      data[ x + sx * y ] = 255;

  tx = sx;
  ty = sy;
  delete[] olddata;
}


