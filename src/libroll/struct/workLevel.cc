/***************************************************************************
                          workLevel.cc  -  description
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


#include <roll/struct/workLevel.h>
#include <roll/struct/general.h>
#include <roll/game/constantes.h>
#include <roll/game/vars.h>
#include <roll/game/roll.h>
#include <roll/game/playerServer.h>
#include <roll/struct/seriesAtari.h>
#include <roll/struct/simpleLevel.h>
#include <roll/struct/levelAtari.h>
#include <roll/game/foregroundElem.h>

#include <time.h>
#include <math.h>

using namespace roll;
using namespace std;


namespace roll
{
  RExplosions	explo;
  BlobDescr	bebet[24];
  RElemFactory	elFactory;
  RBackFactory	backFactory;
}


WorkLevel::WorkLevel() 
  : d( 0 ), b( 0 ), players( PlayerServer::server ), _modified( false )
{
}



WorkLevel::~WorkLevel()
{
  deleteData();
}


void WorkLevel::deleteData()
{
  unsigned	i, j;

  if( d )
    {
      for( i=0; i<sizeX(); ++i )
	{
	  for( j=0; j<sizeY(); ++j )
	    if( d[i][j] ) 
	      delete d[i][j];

	  delete[] d[ i ];
	}

      delete[] d;
      d = 0;
    }

  if( b )
    {
      for( i=0; i<sizeX(); ++i )
	{
	  for( j=0; j<sizeY(); ++j )
	    if( b[i][j] ) 
	      delete b[i][j];

	  delete[] b[ i ];
	}

      delete[] b;
      b = 0;
    }
}


void WorkLevel::allocData( unsigned dx, unsigned dy )
{
  unsigned	i, j;

  deleteData();

  _sx = dx;
  _sy = dy;

  d = new GElem**[ dx ];

  for( i=0; i<dx; ++i )
    {
      d[i] = new GElem*[ dy ];
      for( j=0; j<dy; ++j )
	d[i][j] = 0;		// anti-bug
    }

  b = new RBack**[ dx ];

  for( i=0; i<dx; ++i )
    {
      b[i] = new RBack*[ dy ];
      for( j=0; j<dy; ++j )
	b[i][j] = 0;		// anti-bug
    }
}


void WorkLevel::make( SimpleLevel & sl )
{
  //cout << "WorkLevel::make( SimpleLevel )\n";
  unsigned			i, j;
  unsigned short		spr;
  PlayerServer::iterator	ip, fp=players->end();

  if( players->empty() )
    (*players)[0];

  for( ip=players->begin(); ip!=fp; ++ip )
    (*ip).second.init();

  doorOpen = false;

  deleteData();
  *(LevelParam *) this = sl;
  tab = sl.srcLevel;
  startPos.erase( startPos.begin(), startPos.end() );

  // conversion of color codes
  _colors[ UNCHANGED ] = RGB( 0, 0, 0 );
  _colors[ ROCK      ] = convertedColor( _colwall );
  _colors[ DIAM      ] = convertedColor( _coldiam );
  _colors[ ROLL      ] = convertedColor( _colroll );
  _colors[ MONSTER   ] = convertedColor( _colleftmonster );
  _colors[ GRASS     ] = convertedColor( _colgrass );

  setConvertedPermeability( _permeability );
  setConvertedSpeed( _speed );
  setConvertedStrength( _strength );
  setConvertedGenSpeed( _genspeed );

  allocData( sizeX(), sizeY() );

  x_tel = y_tel = 0;	// teleporter exit

  //	Level data conversion

  GElem			*el;
  Roll			*el2;
  RBack			*bk;
  unsigned		nj;
  unsigned short	*front = sl.foreground();
  unsigned short	*back = sl.background();
  if( !back )
    back = front;

  for( j=0; j<_sy; j++ )
    for( i=0; i<_sx; ++i )
      {
	spr = sl.foreground()[ i + j*_sx ];
	el = elFactory.createElem( spr );
	bk = backFactory.createBack( spr );
	d[i][j] = el;
	b[i][j] = bk;
	if( spr == 192 && j < _sy-1 )	// teleporter exit
	  {
	    x_tel = i;
	    y_tel = j;
	  }
	if( el->isRoll() )
	  startPos.push_back( Coord( i, j ) );
      }

  //	Borders: background is indestructible

  for( i=0; i<_sx; i++ )
    {
      b[i][0]->f |= BI_INDE;
      b[i][_sy-1]->f |= BI_INDE;
    }
  for( j=1; j<_sy-1; j++ )
    {
      b[0][j]->f |= BI_INDE;
      b[_sx-1][j]->f |= BI_INDE;
    }

  //	Rolls

  Roll	**rls = new Roll*[ startPos.size() ];

  for( nj=0; nj<startPos.size(); ++nj )
    rls[nj] = (Roll *) d[ startPos[nj].x ][ startPos[nj].y ];

  if( startPos.size() <= players->size() )	// too many players
    {
      for( nj=0, ip=players->begin(); nj<startPos.size(); ++nj, ++ip )
	{
	  rls[nj]->setNum( (*ip).first );
	  (*ip).second.pos = startPos[nj];
	}
      for( nj=startPos.size(); ip!=fp; ++nj, ++ip )
	{
	  el2 = new Roll( 256 + 34 );	// pour l'instant
	  el2->setNum( (*ip).first );
	  if( !startPos.empty() )
	    {
	      j = nj % startPos.size();	// no départ
	      rls[ j ]->setBackRoll( el2 );
	      rls[ j ] = el2;
	      (*ip).second.pos = startPos[ j ];
	    }
	}
    }
  else	// not enough players
    {
      vector<unsigned>	fs, os;

      for( nj=0, ip=players->begin(); nj<startPos.size(); ++nj, ++ip )
	if( rls[nj]->s < 0x100 )	// mandatory start
	  {
	    if( ip == fp )
	      ip = players->begin();
	    os.push_back( nj );
	    rls[nj]->setNum( (*ip).first );
	    (*ip).second.pos = startPos[nj];
	  }
	else	// optional
	  {
	    fs.push_back( nj );
	  }

      j = 0;

      if( os.size() < players->size() )
	for( ; ip!=fp; ++j, ++ip )
	  {
	    rls[ fs[j] ]->setNum( (*ip).first );
	    (*ip).second.pos = startPos[ fs[j] ];
	  }

      for( ; j<fs.size(); ++j )
	{
	  delete rls[ fs[j] ];	// remove overnumber players
	  d[ startPos[ fs[j] ].x ][ startPos[ fs[j] ].y ] = new EmptyElem;
	}
    }

  delete[] rls;

  //	Copy default explosions
  explo = expli;

  //	Copy default blobs
  for( i=0; i<24; ++i )
    bebet[i] = bebi[i];

  //	Variable level parameters

  SimpleLevel::ParamCode	pa;
  unsigned short		adr, ad, k, l;
  unsigned short		nr;
  unsigned			ipa, n = sl.params.size();

  params = sl.params;	// keep a copy of them

  //cout << "num params : " << n << endl;
  for( ipa=0; ipa<n; ++ipa )
    {
      pa = sl.params[ipa].first;
      //cout << "param " << ipa << " : code " << (int) pa << endl;
      const vector<unsigned short>	& pb = sl.params[ipa].second;
      switch( pa )
	{
	case SimpleLevel::Explosions:
	  nr = pb[0];
	  adr = 1;
	  //cout << "Explosions: " << nr << "\n";
	  for( j=0; j<nr; ++j )
	    {
	      ad = explo.a[ pb[ adr++ ] ]; // element
	      adr += 4; // skip sizes and position for now
	      for( k=0; k<3; k++ ) for( l=0; l<3; l++ )
		explo.d[ad][l][k] = pb[ adr++ ];
	    }
	  break;
	case SimpleLevel::Blobs:
	  nr = pb[0];
	  adr = 1;
	  //cout << "Blobs: " << nr << "\n";
	  for( j=0; j<nr; ++j )
	    {
	      ad = explo.a[ pb[ adr++ ] ];
	      bebet[ad].tmax = pb[ adr++ ];
	      adr += 2; // skip size
	      for( k=0; k<3; k++ ) for( l=0; l<3; l++ )
		bebet[ad].d1[l][k] = pb[ adr++ ];
	      adr += 2; // skip size
	      for( k=0; k<3; k++ ) for( l=0; l<3; l++ )
		bebet[ad].d2[l][k] = pb[ adr++ ];
	    }
	  break;
	default:
	  break;
	}
    }
  setModified( false );
  //cout << "make finished\n";
}


WorkLevel::RGB WorkLevel::convertedColor( unsigned short code )
{
  RGB	rgb;

  rgb.r = ( ((short)(code & 0xf00)) - 0x800 ) / 8;
  rgb.g = ( ((short)(code & 0xf0)) - 0x80 ) * 2;
  rgb.b = ( ((short)(code & 0xf)) - 0x8 ) * 32;

  /*rgb.r = 4.89e-4 * ( code & 0xf00 );
  rgb.g = 7.82e-3 * ( code & 0xf0 );
  rgb.b = 0.126 * ( code & 0xf );*/

  return( rgb );
}


void WorkLevel::resetScores()
{
  PlayerServer::iterator	ip, fp=players->end();

  for( ip=players->begin(); ip!=fp; ++ip )
    (*ip).second.reset();
}


bool WorkLevel::decode( SimpleLevel & sl, int maxLayers, 
			bool /*wantFlags*/ ) const
{
  sl = SimpleLevel( sizeX(), sizeY(), maxLayers > 1 || maxLayers < 0, false );
  (LevelParam &) sl = *this;
  // current strength, speed params are not valid (converted, maybe modified)
  // so we restore the original level ones
  sl._permeability = tab->permeability();
  sl._speed = tab->speed();
  sl._strength = tab->strength();
  sl._genspeed = tab->genSpeed();

  unsigned short	*fg = sl.foreground();
  unsigned short	*bg = sl.background();
  unsigned		i, x, y, nx = sizeX(), ny = sizeY();

  for( i=0, y=0; y<ny; ++y )
    for( x=0; x<nx; ++x, ++i )
      {
	fg[i] = d[x][y]->idNumber();
	if( bg )
	  bg[i] = b[x][y]->idNumber();
	else if( fg[i] == 255 )
	  fg[i] = b[x][y]->idNumber();
      }

  //	variable params: just copy them back
  sl.params = params;

  return( true );
}


void WorkLevel::resize( unsigned sx, unsigned sy )
{
  if( sx == sizeX() && sy == sizeY() )
    return;

  GElem ***fg = d;
  RBack ***bg = b;

  d = 0;
  b = 0;
  unsigned	osx = sizeX(), osy = sizeY();
  unsigned	i, j, dx = sx, dy = sy;

  allocData( sx, sy );

  if( dx > osx )
    dx = osx;
  if( dy > osy )
    dy = osy;

  // move elements

  for( j=0; j<dy; ++j )
    {
      for( i=0; i<dx; ++i )
	{
	  d[i][j] = fg[i][j];
	  fg[i][j] = 0;
	  b[i][j] = bg[i][j];
	  bg[i][j] = 0;
	}
      if( j == 0 || j == sy-1 )
	for( ; i<sx-1; ++i )
	  {
	    d[i][j] = new Wall( 20 );
	    b[i][j] = new RBack;
	    b[i][j]->f |= BI_INDE;
	  }
      else
	for( ; i<sx-1; ++i )
	  {
	    d[i][j] = new EmptyElem;
	    b[i][j] = new RBack;
	  }
      if( sx != osx )
	{
	  d[sx-1][j] = new Wall( 20 );
	  b[sx-1][j] = new RBack;
	  b[sx-1][j]->f |= BI_INDE;
	}
    }
  for( ; j<sy-1; ++j )
    {
      d[0][j] = new Wall( 20 );
      b[0][j] = new RBack;
      b[0][j]->f |= BI_INDE;
      for( i=1; i<sx-1; ++i )
	{
	  d[i][j] = new EmptyElem;
	  b[i][j] = new RBack;
	}
      d[sx-1][j] = new Wall( 20 );
      b[sx-1][j] = new RBack;
      b[sx-1][j]->f |= BI_INDE;
    }
  if( sy != osy )
    for( i=0; i<sx; ++i )
      {
	d[i][sy-1] = new Wall( 20 );
	b[i][sy-1] = new RBack;
	b[i][sy-1]->f |= BI_INDE;
      }

  // delete old data

  if( fg )
    {
      for( i=0; i<osx; ++i )
	{
	  for( j=0; j<osy; ++j )
	    delete fg[i][j];

	  delete[] fg[ i ];
	}

      delete[] fg;
    }

  if( bg )
    {
      for( i=0; i<osx; ++i )
	{
	  for( j=0; j<osy; ++j )
	      delete bg[i][j];

	  delete[] bg[ i ];
	}

      delete[] bg;
    }

  setModified( true );
}


void WorkLevel::setConvertedPermeability( unsigned char x )
{
  if( x != 0 )
    _permeability = (unsigned short) 
      ( (float)exp( 5.5 * log((float) x) ) * 6.9e-7 ) >> 8;
  else
    _permeability = 0;
}


void WorkLevel::setConvertedSpeed( unsigned char x )
{
  if( x != 0 )
    _speed = (unsigned short) ( (float)exp( 8. * log((float) x) ) * 7.1e-12 ) 
      >> 8;
  else
    _speed = 0;
}


void WorkLevel::setConvertedStrength( unsigned char x )
{
  if( x != 0 )
    _strength = (unsigned short) ( (float)exp( 2.5 * log((float) x) )
				   * 0.67 ) >> 8;
  else
    _strength = 0;
}


void WorkLevel::setConvertedGenSpeed( unsigned char x )
{
  if( x != 0 )
    _genspeed = (unsigned short) ( (float)exp( 5.5 * log((float) x) )
				   * 6.9e-7 ) >> 8;
  else
    _genspeed = 0;
}


void WorkLevel::setColor( PartColor index, const RGB col )
{
  _colors[ index ] = col;
  unsigned short code = ( ( col.r + 0x100 ) << 3 ) & 0xf00;
  code |= ( ( col.g + 0x100 ) >> 1 ) & 0xf0;
  code |= ( ( col.b + 0x100 ) >> 5 ) & 0Xf;
  switch( index )
    {
    case ROCK:
      setWallColor( code );
      break;
    case DIAM:
      setDiamColor( code );
      break;
    case ROLL:
      setRollColor( code );
      break;
    case MONSTER:
      setLeftMonsterColor( code );
      break;
    case GRASS:
      setGrassColor( code );
      break;
    default:
      break;
    }
}


