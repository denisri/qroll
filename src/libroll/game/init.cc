/***************************************************************************
                          init.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2006 by Denis RiviÃ¯Â¿Â½e
    email                : nudz@free.fr
                           http://nudz.free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <roll/game/vars.h>
#include <roll/game/init.h>
#include <roll/game/random.h>
#include <roll/struct/seriesAtari.h>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


using namespace roll;
using namespace std;

namespace roll
{
  string	RR_path;
  RExplosions	expli;
  BlobDescr	bebi[24];
  SpritFlag	sp_flg[512];

  string rrVersionString()
  {
    return "1.0";
  }

}

namespace
{
  string & _argv0()
  {
    static string argv0;
    return argv0;
  }
}

void roll::setArgv0( const char* argv0 )
{
  _argv0() = argv0;
}


string roll::argv0()
{
  return _argv0();
}


namespace
{

  string followlink( const string & path )
  {
#ifndef _WIN32
    size_t len = 256;
    vector<char>  pbuf( len );
    errno = 0;
    ssize_t blen = readlink( path.c_str(), &pbuf[0], len );
    while( blen < 0 && errno == ENAMETOOLONG )
    {
      len *= 2;
      pbuf.reserve( len );
      pbuf.resize( len );
      errno = 0;
      blen = readlink( path.c_str(), &pbuf[0], len );
    };
    errno = 0;
    if( blen > 0 )
    {
      pbuf[blen] = '\0';
      cout << &pbuf[0] << endl;
      return &pbuf[0];
    }
#endif
    return path;
  }


  string abspath( string path )
  {
    if( path.empty() || path[0] == '/' )
      return path;

    char *paths = getenv( "PATH" );
    if( paths )
    {
      string spath = paths;
      string::size_type pos0 = 0, pos = spath.find( ':' ), 
          len = spath.length();
      string  ptry;
      struct stat stats;
      while( pos < len && pos0 <= len )
      {
        ptry = spath.substr( pos0, pos - pos0 );
        if( !ptry.empty() )
        {
          ptry += '/';
          ptry += path;
          if( !stat( ptry.c_str(), &stats ) 
               && ( stats.st_mode & S_IFREG ) )
          {
            path = ptry;
            break;
          }
        }
        pos0 = pos + 1;
        pos = spath.find( ':', pos0 );
      }
    }
    if( !path.empty() && path[0] != '/' )
    {
      errno = 0;
      string::size_type len = 256;
      vector<char> pbuf( len );
      char *l = getcwd( &pbuf[0], len );

      while( errno == ERANGE )
      {
        errno = 0;
        len *= 2;
        pbuf.resize( len );
        l = getcwd( &pbuf[0], len );
      }
      if( l )
        path = l + '/' + path;
    }
    return path;
  }


  string qRollSharePathP()
  {
    static string RR_path;

    if( !RR_path.empty() )
      return RR_path;

    char *path = getenv( "ROLLDIR" );

#ifdef _WIN32
    //ofstream  tmp( "toto.txt" );
#endif
    string argv0 = _argv0();

    if( path != 0 )
      RR_path = path;
    else
      if( !argv0.empty() )
        {
          string  comp = argv0; // command invoked with full path ?
          string::size_type  pos;

#ifdef _WIN32
          //tmp << comp << endl;
          char  *cmd = getenv( "CMDLINE" );
          if( cmd )
          {
            string  cms = cmd;
            //tmp << cms << endl;
            pos = cms.rfind( '\\' );
            if( pos == string::npos )
              pos = cms.rfind( '/' );
            //tmp << pos << endl;
            if( pos != string::npos )
              RR_path = cms.substr( 0, pos+1 ) + "share/qroll";
          }
          if( RR_path.empty() )
            RR_path = "share/qroll";
          //tmp << RR_path << endl;
#else // _WIN32

          comp = followlink( abspath( comp ) );

#ifdef __APPLE__
          pos = comp.rfind( "/MacOS" );
#else
          pos = comp.rfind( "/bin/" );
          if( pos == string::npos )
            pos = comp.rfind( '/' );
#endif
          if( pos != string::npos )
            RR_path = comp.substr( 0, pos+1 ) + "share/qroll";
          else
            RR_path = "/usr/local/share/qroll";
#endif  // _WIN32
        }
      else
        RR_path = "/usr/local/share/qroll";

    string  path2 = RR_path + "-" + rrVersionString();
    struct stat s;
    if( !stat( path2.c_str(), &s ) && ( s.st_mode & S_IFDIR ) )
      RR_path = path2;
    return RR_path;
  }

}


string roll::qRollSharePath()
{
  return qRollSharePathP();
}


void init( int, char** )
{
  cout << "init...\n";
  ser = new SeriesAtari;

  RR_path = qRollSharePathP();
  out << "RR PATH = " << RR_path << endl;

  load_sprlink();		// Table de conversion des sprites
  load_spflag();		// CaractÃ¯Â¿Â½istiques des sprites
  expli.load( (RR_path + "/data/explode.b").c_str() );	// Explosions
  load_beb();
  if( !rrand.loadTable( RR_path + "/data/random.b" ) )
    {
      //tmp << "Random table couldn't load - generating a new one\n";
      cerr << "Random table couldn't load - generating a new one\n";
      rrand.genTable( 128*1024 );	// 128K should be OK
      rrand.saveTable( RR_path + "/data/random.b" );	// attempt to save it
    }
  //else tmp << "random table loaded\n";
}


void exit_cleanup()
{
}


void load_sprlink()
{
  ifstream	fich( (RR_path + "/data/spritdef.b").c_str(), 
		      ios::in | ios::binary );
  if( !fich )
    {
      out << "spritedef.b pas trouvÃ¯Â¿Â½n";
      exit( 1 );
    }
  fich.unsetf( ios::skipws );

  for( int i=0; i<960; i++ ) fich >> spr_link[i];
}


void load_spflag()
{
  ifstream	fich( (RR_path + "/data/spritcar.b").c_str() );
  if( !fich )
    {
      out << "spritcar.b not found\n";
      exit( 1 );
    }

  string	str;

  for( int i=0; i<512; i++ )
    {
      fich >> str;
      sscanf( str.c_str(), "%lX", &sp_flg[i].l1 );
      fich >> str;
      sscanf( str.c_str(), "%lX", &sp_flg[i].l2 );
    }
}


void load_beb()
{
  unsigned char			c1, c2;
  unsigned short		spr, ad=0;

  //	out << "Chargement des bÃ¯Â¿Â½Ã¯Â¿Â½es\n";

  ifstream	fich( (RR_path + "/data/bebetes.b").c_str(), 
		      ios::in | ios::binary );
  if( !fich )
    {
      out << "bebetes.b not found - aborting\n";
      exit( 1 );
    }
  fich.unsetf( ios::skipws );
  int			i, j;

  while( !fich.eof() )
    {
      fich >> c1 >> c2 >> c1 >> c2 >> c1 >> c2;
      spr = ((unsigned short)c1<<8) + c2 - 1;
      //		out << "Beb " << ad << " : spr " << spr << "\n";
      if( spr > 0x100+239 ) break;
      expli.a[spr] = ad;
      expli.a[spr+1] = ad;
      expli.a[spr+2] = ad;

      fich >> c1 >> c2;
      bebi[ad].tmax = ((unsigned short)c1<<8) + c2;

      for( j=0; j<3; j++ ) for( i=0; i<3; i++ )
	{
	  fich >> c1 >> c2;
	  spr = ((unsigned short)c1<<8) + c2;
	  bebi[ad].d1[i][j] = spr;
	}
      for( j=0; j<3; j++ ) for( i=0; i<3; i++ )
	{
	  fich >> c1 >> c2;
	  spr = ((unsigned short)c1<<8) + c2;
	  bebi[ad].d2[i][j] = spr;
	}
      bebi[ad].cpt = 0;
      bebi[ad].flg = 0;
      bebi[ad].blk = 0;
      bebi[ad].exi = 0;

      ++ad;
    }
}






