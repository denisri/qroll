/***************************************************************************
                          gameConfig.cc  -  description
                             -------------------
    begin                : 2007
    copyright            : (C) 2001-2007 by Denis Riviere
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

#include <roll/gui/gameConfig.h>
#include <roll/gui/mainWin.h>
#include <roll/gui/qglGameField.h>
#include <roll/game/game.h>
#include <roll/game/vars.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace roll;
using namespace std;


struct GameConfig::Private
{
  Private();

  bool needsSave;
  bool usesCustomTiming;
  bool allowZoom;
  bool fullScreen;
  bool mute;
};


GameConfig::Private::Private()
  : needsSave( false ), usesCustomTiming( false ), allowZoom( false ), 
    fullScreen( false ), mute( false )
{
}


GameConfig & GameConfig::config()
{
  static GameConfig gc;
  return gc;
}


string GameConfig::homedir()
{
  char  *path = getenv( "HOME" );
  if( path )
    return path;
#ifdef _WIN32
  return "C:\\";
#else
  return "/";
#endif
}


string GameConfig::configDir()
{
  return homedir() + "/.qroll";
}


string GameConfig::configFilename()
{
  return configDir() + "/qrollrc";
}


GameConfig::GameConfig()
  : d( new Private )
{
}


GameConfig::~GameConfig()
{
  delete d;
}


void GameConfig::setAllRollsMustExit( bool x )
{
  game.allRollsMustExit = x;
}


bool GameConfig::allRollsMustExit() const
{
  return game.allRollsMustExit;
}


void GameConfig::setUseOpenGL( bool x )
{
#ifndef RR_NO_OPENGL
  theQRWin->setUseOpenGL( x );
#endif
}


bool GameConfig::usesOpenGL() const
{
#ifndef RR_NO_OPENGL
  return theQRWin->usesOpenGL();
#else
  return false;
#endif
}


void GameConfig::setGLMode( int m )
{
#ifndef RR_NO_OPENGL
  QRGLGameField::setGLMode( (QRGLGameField::GLMode) m );
#endif
}


int GameConfig::glMode() const
{
#ifndef RR_NO_OPENGL
  return QRGLGameField::glMode();
#else
  return 0;
#endif
}


void GameConfig::setUseCustomTiming( bool x )
{
  d->usesCustomTiming = x;
  if( !x )
    theQRWin->setTempo( 75 );
}


bool GameConfig::usesCustomTiming() const
{
  return d->usesCustomTiming;
}


void GameConfig::setTiming( int x )
{
  if( x > 0 )
    theQRWin->setTempo( (unsigned) x );
}


int GameConfig::timing() const
{
  return theQRWin->tempo();
}


bool GameConfig::allowZoom() const
{
  return d->allowZoom;
}


void GameConfig::setAllowZoom( bool x )
{
  if( x != d->allowZoom )
  {
    d->allowZoom = x;
    theQRWin->allowScale();
  }
}


bool GameConfig::fullScreen() const
{
  return d->fullScreen;
}


void GameConfig::setFullScreen( bool x )
{
  if( x != d->fullScreen )
  {
    d->fullScreen = x;
    // theQRWin->
  }
}


bool GameConfig::mute() const
{
  return d->mute;
}


void GameConfig::setMute( bool x )
{
  if( x != d->mute )
  {
    d->mute = x;
    theQRWin->soundOnOff();
  }
}


namespace
{

  string getline( istream & f )
  {
    int c = 0;
    bool  last = false, first = true;
    string line;
    while( !f.eof() )
    {
      c = f.get();
      if( c == '\0' || c == '\n' || c == '\r' || c == EOF )
        break;
      if( c == ' ' || c == '\t' )
      {
        if( !first && !last )
        {
          last = true;
          line += ' ';
        }
      }
      else
      {
        last = false;
        first = false;
        line += (char) c;
      }
    }

    return line;
  }


  void readerror( int num, bool & firsterr, const string & msg,
                  const string & line )
  {
    if( firsterr )
    {
      out << "error while reading config file "
          << GameConfig::configFilename() << ":\n";
      firsterr = false;
    }
    out << "line " << num << ": \"" << line << "\"" << endl;
    out << "    " << msg << endl;
  }


  bool str2bool( const string & txt, bool & x )
  {
    if( txt == "0" || txt == "false" || txt == "False" || txt == "FALSE" )
    {
      x = false;
      return true;
    }
    else if( txt == "1" || txt == "true" || txt == "True" || txt == "TRUE" )
    {
      x = true;
      return true;
    }
    return false;
  }


  void allRollsMustExitCbk( const string &, const string & value, int num,
                            const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setAllRollsMustExit( x );
    else
      readerror( num, firsterr,
                 string( "not a boolean value: \"" ) + value + "\"", line );
  }


  void usesOpenGLCbk( const string &, const string & value, int num,
                      const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setUseOpenGL( x );
    else
      readerror( num, firsterr,
                 string( "not a boolean value: \"" ) + value + "\"", line );
  }


  void glModeCbk( const string &, const string & value, int num,
                  const string & line, bool firsterr )
  {
    char  *se = const_cast<char *>( value.c_str() );
    int x = (int) strtol( se, &se, 0 );
    if( se == value.c_str() )
      readerror( num, firsterr,
                 string( "not an integer value: \"" ) + value + "\"", line );
    else
      GameConfig::config().setGLMode( x );
  }


  void usesCustomTimingCbk( const string &, const string & value, int num,
                            const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setUseCustomTiming( x );
    else
      readerror( num, firsterr,
                 string( "not a boolean value: \"" ) + value + "\"", line );
  }


  void timingCbk( const string &, const string & value, int num,
                  const string & line, bool firsterr )
  {
    char  *se = const_cast<char *>( value.c_str() );
    int x = (int) strtol( se, &se, 0 );
    if( se == value.c_str() )
      readerror( num, firsterr,
                 string( "not an integer value: \"" ) + value + "\"", line );
    else
      GameConfig::config().setTiming( x );
  }


  void allowZoomCbk( const string &, const string & value, int num,
                     const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setAllowZoom( x );
    else
      readerror( num, firsterr,
                 string( "not an boolean value: \"" ) + value + "\"", line );
  }


  void muteCbk( const string &, const string & value, int num,
                const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setMute( x );
    else
      readerror( num, firsterr,
                 string( "not an boolean value: \"" ) + value + "\"", line );
  }


  void fullScreenCbk( const string &, const string & value, int num,
                      const string & line, bool firsterr )
  {
    bool x;
    if( str2bool( value, x ) )
      GameConfig::config().setFullScreen( x );
    else
      readerror( num, firsterr,
                 string( "not an boolean value: \"" ) + value + "\"", line );
  }


}


void GameConfig::load()
{
  out << "load config " << configFilename() << endl;
  ifstream f( configFilename().c_str() );
  if( !f )
  {
    out << "config file is not readable or does not exist.\n";
    return; // file could not open: guess it doesn't exist
  }
  string line, key, value;
  string::size_type pos;
  int num = 0;
  bool firsterr = true;

  map<string, void (*)( const string &, const string &, int, const string &,
                        bool )>
      callbacks;
  map<string, void (*)( const string &, const string &, int, const string &,
                        bool )>::const_iterator icbk, nocbk = callbacks.end();
  callbacks[ "allRollsMustExit" ] = &allRollsMustExitCbk;
  callbacks[ "useOpenGL" ] = &usesOpenGLCbk;
  callbacks[ "glMode" ] = &glModeCbk;
  callbacks[ "useCustomTiming" ] = &usesCustomTimingCbk;
  callbacks[ "timing" ] = &timingCbk;
  callbacks[ "allowZoom" ] = &allowZoomCbk;
  callbacks[ "mute" ] = &muteCbk;
  callbacks[ "fullScreen" ] = &fullScreenCbk;

  while( !f.eof() )
  {
    // read a single line
    line = getline( f );
    ++num;
    if( line.length() == 0 || line[0] == '#' )
      continue;
    // look for key demlimitation
    pos = line.find( ':' );
    if( pos == string::npos )
    {
      readerror( num, firsterr, "syntax error", line );
      continue;
    }
    key = line.substr( 0, pos );
    if( line.length() == pos+1
        || ( line.length() == pos+2 && line[pos+1] == ' ' ) )
    {
      readerror( num, firsterr, string( "no value for key '" ) + key
          + "'", line );
      continue;
    }
    if( line[pos+1] == ' ' )
      value = line.substr( pos+2, line.length() - pos - 2 );
    else
      value = line.substr( pos+1, line.length() - pos - 1 );

    // interpret key/value
    icbk = callbacks.find( key );
    if( icbk == nocbk )
    {
      readerror( num, firsterr, string( "unrecognized variable '" ) + key
          + "'", line );
      continue;
    }
    icbk->second( key, value, num, line, firsterr );
  }
}


void GameConfig::save()
{
  out << "save config " << configFilename() << endl;
  struct stat s;
  int r = stat( configDir().c_str(), &s );
  if( r != 0 )
  {
    out << "mkdir " << configDir() << endl;
#ifdef _WIN32
    mkdir( configDir().c_str() );
#else
    mkdir( configDir().c_str(), 0770 );
#endif
  }
  ofstream  f( configFilename().c_str() );
  f << "# Rock'n'Roll configuration file\n";
  f << "# Avoid editing manually since comments will be erased\n";
  f << "allRollsMustExit: " << (int) allRollsMustExit() << endl;
  f << "useOpenGL: " << (int) usesOpenGL() << endl;
  f << "glMode: " << glMode() << endl;
  f << "useCustomTiming: " << usesCustomTiming() << endl;
  f << "timing: " << timing() << endl;
  f << "allowZoom: " << (int) d->allowZoom << endl;
  f << "mute: " << (int) d->mute << endl;
  if( d->fullScreen )
    f << "fullScreen: " << (int) d->fullScreen << endl;
}

