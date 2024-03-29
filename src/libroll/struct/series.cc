/***************************************************************************
                          series.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Riviere
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


#include <roll/struct/series.h>
#include <roll/struct/simpleLevel.h>
#include <roll/struct/workLevel.h>
#include <roll/struct/general.h>
#include <fstream>
#include <stdio.h>
#include <QFile>

using namespace roll;
using namespace std;


static void cleantag( string & s )
{
  if( !s.empty() && ( s[ s.length()-1 ] == ' ' || s[ s.length()-1 ] == '\t'
      || s[ s.length()-1 ] == '\n' ) )
    s.erase( s.length()-1, 1 );
}

Series::Series() : _modified( false )
{
}


Series::~Series()
{
}


Series::Series( const string & name ) : _filename( name )
{
}


bool Series::pack( const WorkLevel & wl, unsigned level )
{
  SimpleLevel	sl;
  if( !wl.decode( sl, levelLayers(), keepsElementFlags() ) )
    return( false );
  if( !pack( sl, level ) )
    return( false );
  setModified( true );
  return( true );
}


void Series::unpack( unsigned level, WorkLevel & wl ) const
{
  SimpleLevel	sl;
  unpack( level, sl );
  wl.make( sl );
}


void Series::resizeSimpleLevel(  unsigned level, SimpleLevel & sl ) const
{
  unsigned	sx, sy;

  size( level, sx, sy );
  sl = SimpleLevel( sx, sy, levelLayers() != 1, keepsElementFlags() );
}


bool Series::save( const string &, const string & ) const
{
  return( false );
}


bool Series::saveRaw( const string & flname ) const
{
  string	fname = flname;

  if( fname.empty() )
    fname = filename();

  ofstream	file( fname.c_str() );
  if( !file )
    return( false );

  unsigned		i, n = numLevels();
  SimpleLevel		sl;

  // determine byte order
  char	bo[5];
  bo[4] = '\0';
  unsigned	bonum = 0x41424344;
  bo[0] = ((char *) &bonum)[0];
  bo[1] = ((char *) &bonum)[1];
  bo[2] = ((char *) &bonum)[2];
  bo[3] = ((char *) &bonum)[3];

  file << "ROCK'N'ROLL Series V1.0\n";
  file << "ByteOrder: " << bo << endl;
  file << "Levels: " << n << endl;

  for( i=0; i<n; ++i )
    {
      unpack( i, sl );
      file << "[Level]\n";
      file << "Level: " << i << endl;
      sl.write( file );
    }

  clearModified();
  return( true );
}

namespace
{
  string readString( QFile & file )
  {
    string str;
    char c = '.';
    while( !file.atEnd() )
    {
      if( !file.getChar( &c ) )
      {
        err << "error reading series " << file.fileName().toStdString() << endl;
        return str;
      }
      if( c == '\0' || c == '\n' || c == ' ' || c == '\t' || c == '\r' )
        break;
      str += c;
    }
    return str;
  }

  unsigned readUInt( QFile & file, bool & ok )
  {
    QString str;
    char c = '.';
    while( !file.atEnd() )
    {
      if( !file.getChar( &c ) )
      {
        err << "error reading series " << file.fileName().toStdString() << endl;
        ok = false;
        return 0;
      }
      if( c < '0' || c > '9' )
        break;
      str += c;
    }
    ok = true;
    return str.toUInt( &ok );
  }
}

bool Series::loadRaw( const string & fname )
{
  QFile	file( fname.c_str() );

  if( !file.open( QIODevice::ReadOnly ) )
  {
    err << fname << " not found\n";
    return false;
  }

  unsigned	n = 20;
  vector<char>	buf( n+1 );

  qint64 l = file.read( &buf[0], n );
  if( l != n )
    out << "mismatch\n";
  buf[ n ] = '\0';
  if( file.atEnd() || string( &buf[0] ) != "ROCK'N'ROLL Series V" )
    // NOT a new format file
    return false;

  string	version = readString( file );
  cleantag( version );
  unsigned	major = 0, minor = 0;
  sscanf( version.c_str(), "%u.%u", &major, &minor );
  if( major == 0 )
    return false;	// failed reading ?
  out << "R&R Raw format series, version " << major << "." << minor << endl;
  if( major != 1 || minor > 0 )
    err << "version mismatch - expecting 1.0,\n" 
      << "format may be incompatible and load may fail\n";

  unsigned	nlevel = 0, i;
  unsigned	bo = 0x41424344;
  string	tag;

  tag = readString( file );
  cleantag( tag );
  bool ok = true;

  //	read header
  while( !file.atEnd() && tag != "[Level]" )
  {
    if( tag == "ByteOrder:" )
    {
      tag = readString( file );
      cleantag( tag );
      if( tag.length() != 4 )
        err << "invalid ByteOrder value\n";
      else
        bo = *(unsigned *)&tag[0];
    }
    else if( tag == "Levels:" )
    {
      nlevel = readUInt( file, ok );
      if( !ok )
        return false;
      out << "Levels : " << nlevel << endl;
    }
    else
    {
      err << "ungnown tag " << tag << endl;
      return( false );
    }
    tag = readString( file );
    cleantag( tag );
  }

  if( file.atEnd() || !file.isOpen() )
  {
    err << "premature EOF - corrupt file\n";
    return false;
  }
  if( nlevel == 0 )
  {
    err << "levels number couldn't be read - aborting\n";
    return false;
  }
  if( bo == 0 )
    err << "warning, byte order not specified - won't byteswap data\n";

  //	read levels
  // up to now levels numbers must be consecutive - may change in the future
  unsigned	j;
  SimpleLevel	sl;

  for( i=0; i<nlevel; ++i )
  {
    tag = readString( file );
    cleantag( tag );
    if( tag == "[Level]" )
    {
      tag = readString( file );
      cleantag( tag );
    }
    if( tag != "Level:" )
    {
      err << "unexpected tag " << tag << "<--" << endl;
      err << "(len: " << tag.length() << " )\n";
      err << "1st char: " << (int) tag[0] << endl;
      return false;
    }
    j = readUInt( file, ok );
    if( !ok )
      return false;
    if( j != i )
      err << "warning : found level " << j << ", expected " << i << endl;
    if( !sl.read( file, bo ) )
    {
      err << "Failed to read level " << j << endl;
      return false;
    }
    if( appendLevel( sl ) == -1 )
      err << "warning : couldn't insert level " << j
        << ", level not loaded\n";
  }
  out << "loading " << fname << " OK.\n";

  return true;
}


int Series::appendLevel( const SimpleLevel & )
{
  return( -1 );
}


int Series::insertLevel( int, const SimpleLevel & sl )
{
  return( appendLevel( sl ) );
}


bool Series::deleteLevel( unsigned )
{
  return( false );
}


