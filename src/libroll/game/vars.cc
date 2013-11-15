/***************************************************************************
                          vars.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2004 by Denis RiviÃ¯Â¿Â½re
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


#include <roll/game/vars.h>
#include <roll/game/playerServer.h>
#include <fstream>

using namespace roll;
using namespace std;

#ifdef _WIN32
// uncomment next line for debug output in file
//#define FILE_OUTPUT
#endif
#ifdef ANDROID
#define FILE_OUTPUT
#endif

namespace roll
{
#ifdef FILE_OUTPUT
#ifdef ANDROID
  static ofstream	qroll_file( "/sdcard/share/qroll/qroll-log.log" );
#else
  static ofstream	qroll_file( "qroll_out.txt" );
#endif
  ostream	& out = qroll_file;
  ostream	& err = qroll_file;
#else
  ostream	& out = cout;
  ostream	& err = cerr;
#endif
  Series	*ser;
  unsigned char	spr_link[960];
  PlayerServer *PlayerServer::server = 0;
  Game		game;

  Game & theGame()
  {
    return game;
  }

}


