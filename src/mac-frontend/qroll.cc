/***************************************************************************
                          qroll.cc
                             -------------------
    begin                : 2002
    copyright            : (C) 2002-2006 by Denis Rivière
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

#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;

int main( int argc, char** argv )
{
    string	name = argv[0];
    unsigned	pos = name.rfind( "MacOS/" );
    if( pos != string::npos )
      {
        name.erase( pos, name.length() - pos );
        string	dep = name, pth = name;
        name += "lib";
        dep += "share/qroll";
        //if( !getenv( "ROLLDIR" ) )
          setenv( "ROLLDIR", dep.c_str(), 1 );
        char	*dlib = getenv( "DYLD_LIBRARY_PATH" );
        if( dlib )
          name += string( ":" ) + dlib;
        setenv( "DYLD_LIBRARY_PATH", name.c_str(), 1 );
        pth += "MacOS/qroll-bin";
        argv[0] = strdup( pth.c_str() );
        execv( pth.c_str(), argv );
      }
}

