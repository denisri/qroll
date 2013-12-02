/***************************************************************************
                          explosions.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis RiviÃ¨re
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


#include <roll/struct/explosions.h>
#include <roll/struct/general.h>
#include <fstream>
#ifdef ANDROID
#include <QFile>
#endif

using namespace std;
using namespace roll;


void RExplosions::load( const string & nom )
{
#ifdef ANDROID
  QFile		fich( nom.c_str() );
  if( !fich.open( QIODevice::ReadOnly ) )
  {
    out << "could not read " << nom << endl;
    return;
  }
  int			i, j, ad=0;
  unsigned char		c1,c2;
  unsigned short	spr;

  for( i=0; i<512; i++ ) a[i] = 0;

  while( !fich.atEnd() )
  {
    if( !fich.getChar( (char *) &c1 ) || !fich.getChar( (char *) &c2 ) )
    {
      out << "error reading explosions\n";
      exit( 1 );
    }
    if( fich.atEnd() ) break;
    spr = ((unsigned short)c1<<8) + c2 - 1;
    if( spr > 239+0x100 ) break;
    a[ spr ] = ad;
    for( j=0; j<3; j++ ) for( i=0; i<3; i++ )
    {
      if( !fich.getChar( (char *) &c1 ) || !fich.getChar( (char *) &c2 ) )
      {
        out << "error reading explosions\n";
        exit( 1 );
      }
      d[ad][i][j] = ((unsigned short)c1<<8) + c2;
    }
    ++ad;
  }

#else

  ifstream		fich( nom.c_str(), ios::in | ios::binary );
  fich.unsetf( ios::skipws );
  int			i, j, ad=0;
  unsigned char		c1,c2;
  unsigned short	spr;

  for( i=0; i<512; i++ ) a[i] = 0;

  while( !fich.eof() )
  {
    fich >> c1 >> c2;
    if( fich.eof() ) break;
    spr = ((unsigned short)c1<<8) + c2 - 1;
    //out << "Expl " << ad << " : sprite " << spr << "\n";
    if( spr > 239+0x100 ) break;
    a[ spr ] = ad;
    for( j=0; j<3; j++ ) for( i=0; i<3; i++ )
	{
	  fich >> c1 >> c2;
	  d[ad][i][j] = ((unsigned short)c1<<8) + c2;
	}
    ++ad;
  }
#endif
  // out << nom << " loaded.\n";

  // Bonhomme sous toutes les formes
  for( j=0; j<6; j++ ) for( i=34; i<36; i++ )
    {
      a[i+j*40] = a[34];
      a[i+j*40+0x100] = a[34];
    }

  // Same for bombs
  a[73] = a[113] = a[33];
  a[0x149] = a[0x171] = a[0x121];
  a[0x1c1] = a[0x1e9] = a[0x199];
  a[0x14c] = a[0x174] = a[0x124];
  // Same for breakable rocks and eggs
  a[0x147] = a[0x16f] = a[0x197] = a[0x11f];
  a[0x1e7] = a[0x1bf];
}


RExplosions & RExplosions::operator = ( RExplosions & exp )
{
  int		i, j;
  unsigned short	spr;

  for( i=0; i<512; i++ ) a[i] = exp.a[i];
  for( spr=0; spr<100; spr++ )
    for( j=0; j<3; j++) for( i=0; i<3; i++ )
      d[spr][i][j] = exp.d[spr][i][j];

  return( *this );
}








