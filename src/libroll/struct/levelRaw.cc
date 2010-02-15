/***************************************************************************
                          levelRaw.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¨re
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

#include <roll/struct/levelRaw.h>
#include <roll/struct/atariFormat.h>
#include <roll/struct/simpleLevel.h>

using namespace roll;
using namespace std;


LevelRaw::LevelRaw() : Level(), LevelParam()
{
}


LevelRaw::LevelRaw( unsigned sx, unsigned sy )
  : Level(), LevelParam(), _data( sx * sy )
{
  _sx = sx;
  _sy = sy;
}


LevelRaw::~LevelRaw()
{
}


unsigned LevelRaw::size() const
{
  return( _data.size() );
}


void LevelRaw::resize( unsigned sx, unsigned sy )
{
  if( sx == sizeX() && sy == sizeY() )
    return;

  vector<unsigned short>	olddata = _data;
  unsigned			x, y, nx = sx, ny = sy;

  _data.clear();
  _data.insert( _data.end(), sx * sy, 0 );

  if( _sx < nx )
    nx = _sx;
  if( _sy < ny )
    ny = _sy;

  for( y=0; y<ny; ++y )
    {
      for( x=0; x<nx; ++x )
	_data[ x + sx * y ] = olddata[ x + _sx * y ];
      for( ; x<sx; ++x )
	_data[ x + sx * y ] = 255;
    }
  for( ; y<sy; ++y )
    for( x=0; x<sx; ++x )
      _data[ x + sx * y ] = 255;

  _sx = sx;
  _sy = sy;
}


bool LevelRaw::load( istream & )
{
  return( false );
}


bool LevelRaw::loadAtari( istream & file, AtariFormatInfo & ai )
{
  //cout << "LevelRaw::loadAtari\n";
  unsigned char	c1,c2;
  unsigned long	i;

  _params.clear();
  _data.clear();

  //	read

  if( !file )
    return( false );
  file >> c1 >> c2;
  _sx = c1;
  _sy = c2;
  //cout << "load level " << _sx << " x " << _sy << endl;
  file >> c1 >> c2;
  _diams = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _time = ((unsigned short)c1<<8) + c2;
  file >> _permeability >> _speed >> _strength >> _genspeed;
  file >> c1 >> c2;
  _colwall = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _coldiam = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _colroll = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _colleftmonster = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _colgrass = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _flags = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _blobmaxsize = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  _bombdelay = ((unsigned short)c1<<8) + c2;
  file >> c1 >> c2;
  unsigned short	debu = ((unsigned short)c1<<8) + c2;

  unsigned 		sparam = debu - 28;
  vector<unsigned char>	pars( sparam );

  for( i=0; i<sparam; ++i )
    file >> pars[i];

  //	convert variable parameters to more readable format

  i=0;
  unsigned short	pa = 1, nr, adr, j, k;
  ParamCode		code;

  while( pa!=0 )
    {
      pa = ((unsigned short) pars[i]<<8) + pars[i+1];
      i += 2;
      adr = ((unsigned short) pars[i]<<8) + pars[i+1] - 26;
      i += 2;
      //cout << "param " << i-4 << ": " << pa << ", adr: " << adr << "\n";
      code = Blobs;
      switch( pa )
	{
	case 3:			// Customized Explosions
	  code = Explosions;
	case 4:
	  {
	    nr = pars[adr-2];
	    _params.push_back( ParamBlock( code, vector<unsigned short>() ) );
	    vector<unsigned short>	& pc = _params.back().second;
	    pc.push_back( nr );
	    for( j=0; j<nr; j++ ) // for each redefined explosion
	      {
		pc.push_back( ((unsigned short) pars[adr]<<8) 
			      + (unsigned short) pars[adr+1] - 1 ); // element
		adr += 2;
		if( code == Blobs )
		  {	// store blob size
		    pc.push_back( ((unsigned short) pars[adr]<<8) 
				  + (unsigned short) pars[adr+1] );
		    adr += 2;
		  }
		pc.push_back( 3 ); // width
		pc.push_back( 3 ); // height
		if( code == Explosions )
		  {
		    pc.push_back( 1 ); // center x
		    pc.push_back( 1 ); // center y
		  }
		for( k=0; k<9; ++k )
		  {
		    pc.push_back( ((unsigned short) pars[adr]<<8)
				  + (unsigned short) pars[adr+1] );
		    adr += 2;
		  }
		if( code == Blobs ) // blocked redefinition
		  {
		    pc.push_back( 3 ); // width
		    pc.push_back( 3 ); // height
		    for( k=0; k<9; ++k )
		      {
			pc.push_back( ((unsigned short) pars[adr]<<8)
				      + (unsigned short) pars[adr+1] );
			adr += 2;
		      }
		  }
	      }
	  }
	  break;
	default:
	  break;
	}
    }

  unsigned	sz = _sx * _sy;

  _data.insert( _data.end(), sz, 0 );

  // read / convert data to decoded format

  for( i=0; i<sz; ++i )
    {
      file >> c1;
      if( c1 < 240 )
	_data[i] = ai.elements_map[ c1 ];
      else
	_data[i] = 255;
    }

  //cout << "end LevelRaw::loadAtari\n";
  return( true );
}


bool LevelRaw::save( ostream & )
{
  return( false );
}


bool LevelRaw::saveAtari( ostream &, AtariFormatInfo & )
{
  return( false );
}


void LevelRaw::unpack( SimpleLevel & sl )
{
  sl = SimpleLevel( sizeX(), sizeY(), false, false );	// up to now: no backg
  (LevelParam &) sl = *this;
  sl.srcLevel = this;

  unsigned short	*front = sl.foreground();
  unsigned		i, n = sizeX() * sizeY();

  sl.params = _params;

  for( i=0; i<n; ++i )
    front[i] = _data[i];
}


bool LevelRaw::pack( const SimpleLevel & sl )
{
  _data.clear();
  *(LevelParam *) this = sl;

  unsigned		i, n = sizeX() * sizeY();
  unsigned short	*fg = sl.foreground();

  _data.insert( _data.end(), n, 255 );
  for( i=0; i<n; ++i )
    _data[i] = fg[i];

  _params = sl.params;

  return( true );
}


