/***************************************************************************
                          simpleLevel.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2000-2005 by Denis Rivière
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

#include <roll/struct/simpleLevel_d.h>
#include <fstream>
#include <string>

using namespace roll;
using namespace std;

namespace roll
{

#ifndef _MSC_VER
/* Microsoft VC++ doesn't like template members specialization. 
   Anyway I prefer the more general way (using base iostream classes rather 
   than fstream) so I persist in using this code under unix */

// compile IO for standard types
template void SimpleLevel::write( ostream & ) const;
template bool SimpleLevel::read( istream &, unsigned );

// use base class for fstreams
template<> void SimpleLevel::write( ofstream & s ) const
{
  write( (ostream &) s );
}


template<> bool SimpleLevel::read( ifstream & s, unsigned byteorder )
{
  return( read( (istream &) s, byteorder ) );
}

#else	// _MSC_VER
#if 0
template void SimpleLevel::write( ofstream & ) const;
template bool SimpleLevel::read( ifstream &, unsigned );
#endif
#endif


void SimpleLevel::makeEmpty()
{
  params.clear();
  delete[] _elemflags;
  _elemflags = 0;

  // standard params
  _diams = 50;
  _time = 100;
  _permeability = 50;
  _speed = 50;
  _strength = 50;
  _genspeed = 50;
  _blobmaxsize = 256;
  _bombdelay = 3;
  _colwall = 0x888;
  _coldiam = 0x808;
  _colroll = 0x06b;
  _colleftmonster = 0x800;
  _colgrass = 0x080;
  _flags = 0;

  unsigned	x, y, nx = sizeX(), ny = sizeY();
  if( ny > 0 )
    {
      for( x=0; x<nx; ++x )
	{
	  _foreground[x] = 20;
	  if( hasBackground() )
	    _background[x] = 255;
	  if( ny > 1 )
	    {
	      _foreground[ x + (ny-1)*nx ] = 20;
	      if( hasBackground() )
		_background[ x + (ny-1)*nx ] = 255;
	    }
	}
      for( y=1; y<ny-1; ++y )
	{
	  if( nx > 0 )
	    {
	      _foreground[y*nx] = 20;
	      if( hasBackground() )
		_background[y*nx] = 255;
	      if( nx > 1 )
		{
		  _foreground[nx-1+y*nx] = 20;
		  if( hasBackground() )
		    _background[nx-1+y*nx] = 255;
		}
	    }
	  for( x=1; x<nx-1; ++x )
	    {
	      _foreground[x+y*nx] = 255;
	      if( hasBackground() )
		_background[x+y*nx] = 255;
	    }
	}
    }
}

}


