/***************************************************************************
                          random.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Rivière
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

#include <roll/game/random.h>
#include <stdlib.h>
#include <stdio.h>


using namespace roll;
using namespace std;


namespace roll
{

  struct Random_Private
  {
    Random_Private() : table( 0 ), pos( 0 ), length( 0 ) {}
    ~Random_Private() { delete[] table; }

    unsigned char	*table;
    unsigned		pos;
    unsigned		length;
  };

};


Random::Random() : d( new Random_Private )
{
}


Random::~Random()
{
  delete d;
}


unsigned char Random::operator() ()
{
  unsigned char	n = *(d->table + d->pos++);
  if( d->pos >= d->length )
    d->pos = 0;
  return( n );
}


void Random::srand( unsigned seed )
{
  d->pos = seed % d->length;
}


bool Random::loadTable( const string & filename )
{
  FILE	*f = fopen( filename.c_str(), "rb" );
  if( !f )
    return( false );
  fseek( f, 0, SEEK_END );
  long	sz = ftell( f );
  unsigned char	*buf = new unsigned char[ sz ];

  rewind( f );
  fread( buf, 1, sz, f );
  fclose( f );

  delete[] d->table;
  d->table = buf;
  d->length = sz;
  d->pos = 0;

  return( true );
}


void Random::genTable( unsigned length )
{
  delete[] d->table;
  d->table = new unsigned char[ length ];
  d->length = length;
  d->pos = 0;

  unsigned	i, l = length / sizeof( int );
  int		*b = (int *) d->table;
  for( i=0; i<l; ++i )
    *b++ = rand();

  unsigned char	*bc = (unsigned char *) b;

  l = length % sizeof( int );
  for( i=0; i<l; ++i )
    *bc++ = (unsigned char) rand() & 0xff;
}


bool Random::saveTable( const string & filename )
{
  FILE	*f = fopen( filename.c_str(), "wb" );

  if( !f )
    return( false );
  fwrite( d->table, 1, d->length, f );
  fclose( f );

  return( true );
}


// ----------


Random	roll::rrand;


