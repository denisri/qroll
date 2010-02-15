/***************************************************************************
                          seriesRaw.cc  -  description
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

#include <roll/struct/seriesRaw.h>
#include <roll/struct/levelRaw.h>
#include <roll/struct/atariFormat.h>
#include <roll/game/vars.h>
#include <fstream>

using namespace roll;
using namespace std;


SeriesRaw::SeriesRaw() : Series()
{
}


SeriesRaw::SeriesRaw( const string & name ) : Series( name )
{
  load( name );
}


SeriesRaw::~SeriesRaw()
{
  clear();
}


void SeriesRaw::clear()
{
  unsigned	i, n = levels.size();
  for( i=0; i<n; ++i )
    delete levels[i];
  levels.clear();
}


void SeriesRaw::size( unsigned level, unsigned & sx, unsigned & sy ) const
{
  const LevelRaw	& lvl = *levels[ level ];
  sx = lvl.sizeX();
  sy = lvl.sizeY();
}


bool SeriesRaw::load( const std::string & name )
{
  cout << "Loading series " << name << "\n";

  if( loadRaw( name ) )
    return( true );
  // NOT a new format file: assuming old Atari format
  return( loadAtari( name ) );
}


bool SeriesRaw::loadAtari( const std::string & name )
{
  //cout << "SeriesRaw::loadAtari...\n";
  int i;

  ifstream	file( name.c_str(), ios::in | ios::binary );

  if( !file )
    {
      cerr << name << " not found\n";
      return( false );
    }
  file.unsetf( ios::skipws );

  unsigned char	c1,c2,c3,c4;
  unsigned long	dec = 0;

  unsigned short	dm, ex, bb, vl;
  unsigned long		cd;

  file >> c1 >> c2;
  dm = ((unsigned short) c1<<8) + c2;
  if( dm >= 1000 )
    return( false ); // probably wrong format
  file >> c1 >> c2;
  ex = ((unsigned short) c1<<8) + c2;
  if( ex >= 1000 )
    return( false );
  file >> c1 >> c2;
  bb=((unsigned short)c1<<8)+c2;
  if( bb >= 1000 )
    return( false );
  file >> c1 >> c2;
  vl=((unsigned short)c1<<8)+c2;
  if( vl > 5 )
    return( false );
  file >> c1 >> c2;
  cd=((unsigned long)c1<<8)+c2;
  file >> c1 >> c2;
  cd+=((unsigned long)c1<<24)+((unsigned long)c2<<16);
  if( cd >= 10000 )
    return( false );

  AtariFormatInfo	atari;

  atari.diams = dm;
  atari.exploding = ex;
  atari.blobs = bb;
  atari.rating = vl;

  //cout << "elements map :\n";
  for(i=0; i<240; i++)
    {
      file >> c1 >> c2;
      atari.elements_map[i]=((unsigned short) c1<<8) + c2;
      if( atari.elements_map[i] >= 256+240 )
	return( false );	// exceeds number of elements in Atari game
      //cout << atari.elements_map[i] << " ";
    }
  //cout << endl;
  convList( atari );

  if( !file )
    return( false );

  file >> c1 >> c2;
  unsigned short	nt = ((unsigned short) c1<<8) + c2;
  if( nt > 1000 )
    return( false );
  cout << "\nLevels : " << nt << "\n\n";

  vector<unsigned long>	fnt( nt );

  // read levels addresses table
  // in fact we don't need it at all
  for( i=0; i<nt; i++ )
    {
      file >> c1 >> c2 >> c3 >> c4;
      fnt[i] = ((unsigned long)c1<<24) | ((unsigned long)c2<<16)
	| ((unsigned long)c3<<8) | (unsigned long)c4;
      if( i==0 ) dec = fnt[0];
      //cout << "level " << i << " : addr = " << fnt[i];
      fnt[i] -= dec;
      //cout << ", rel : " << fnt[i] << endl;
      if( i > 0 && fnt[i] <= fnt[i-1] )
	{
	  /*cout << "fnt[" << (i-1) << "]: " << fnt[i-1]
	       << " >= fnt[" << i << "]: " << fnt[i]
	       << ". cars: "
	       << (int)c1 << " " << (int)c2 << " "
	       << (int)c3 << " " << (int)c4
	       << "\n";*/
	  return( false );
	}
    }

  if( !file )
    return( false );

  cout << "seems to be a valid Atari / Old style 8 bit/elem format...\n";

  //clear();
  unsigned	ont = numLevels();
  levels.insert( levels.end(), nt, 0 );

  for( i=0; i<nt; i++ )
    {
      levels[ont+i] = new LevelRaw;
      if( !levels[ont+i]->loadAtari( file, atari ) )
	{
	  cerr << "Load failed in level " << i << endl;
	  clear();
	  return( false );
	}
      //out << "Tableau " << i << " : taille : " << tb[i].size << "\n";
    }

  setFilename( name );
  setModified( false );

  //cout << "end LoadAtari\n";
  return( true );
}


void SeriesRaw::convList( AtariFormatInfo & ai )
{
  unsigned short	i,j,sp,s,s2;
  unsigned char		sprconv[480];
  unsigned char		spr_on[480];

  for( i=0; i<480; ++i )
    {
      sprconv[i] = 0xFF;
      spr_on[i] = 0;
    }

  //	Roll

  for(i=0; i<6; i++)
    {
      spr_on[ 34+i*40 ] = 0xFF;
      spr_on[ 35+i*40 ] = 0xFF;
      sprconv[ 34+i*40 ] = i*2;
      sprconv[ 35+i*40 ] = i*2+1;
    }

  spr_on[18] = 0xFF;
  spr_on[58] = 0xFF;
  sprconv[18] = 12;
  sprconv[58] = 13;

  sp=14;
  for( i=0; i<240; ++i )
    {
      s = ai.elements_map[i];
      if( s != 0xFF )
	{
	  if( s >= 0x100 )
	    s-=16;
	  s2=s;
	  if( !spr_on[s] )
	    for( j=0; j<(spr_link[2*s] & 0x7F); ++j )
	      {
		spr_on[s2] = 0xFF;
		sprconv[s2] = sp++;
		s2 = spr_link[2*s2+1];
		if( s >= 240 )
		  s2 += 240;
	      }
	}
    }

  for( i=0; i<240; ++i )
    ai.elements_map[i] = 0xFF;

  for( i=0; i<480; ++i )
    if( spr_on[i] )
      {
	if( i < 240 )
	  ai.elements_map[ sprconv[i] ] = i;
	else
	  ai.elements_map[ sprconv[i] ] = i + 16;
      }
}


bool SeriesRaw::save( const std::string & fname, 
		      const std::string & format ) const
{
  if( format == "atari" )
    return( saveAtari( fname ) );
  return( saveRaw( fname ) );
}


bool SeriesRaw::saveAtari( const std::string & ) const
{
  return( false );
}


bool SeriesRaw::pack( const SimpleLevel & sl, unsigned level )
{
  bool	res = levels[ level ]->pack( sl );
  if( res )
    setModified( true );
  return( res );
}


void SeriesRaw::unpack( unsigned level, SimpleLevel & sl ) const
{
  levels[ level ]->unpack( sl );
}


const Level* SeriesRaw::level( unsigned x ) const
{
  return( levels[x] );
}


Level* SeriesRaw::level( unsigned x )
{
  return( levels[x] );
}


int SeriesRaw::appendLevel( const SimpleLevel & sl )
{
  LevelRaw	*lev = new LevelRaw;
  lev->pack( sl );
  levels.push_back( lev );
  setModified( true );
  return( levels.size() - 1 );
}


int SeriesRaw::insertLevel( int pos, const SimpleLevel & sl )
{
  if( pos < 0 || pos >= (int) levels.size() )
    return( appendLevel( sl ) );

  LevelRaw	*lev = new LevelRaw;
  lev->pack( sl );
  levels.insert( levels.begin() + pos, lev );
  setModified( true );
  return( pos );
}


bool SeriesRaw::deleteLevel( unsigned pos )
{
  if( pos >= levels.size() )
    return( false );
  levels.erase( levels.begin() + pos );
  setModified( true );
  return( true );
}


