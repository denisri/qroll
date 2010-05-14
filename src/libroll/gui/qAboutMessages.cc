/***************************************************************************
qAbout.cc  -  description
-------------------
begin                : 1999
copyright            : (C) 1999-2005 by Denis Rivière
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


#include "qAbout.h"
#include <roll/gui/mainWin.h>
#include <roll/game/vars.h>
#include <sys/stat.h>

using namespace roll;
using namespace std;


QString QAbout::scrollingMessageFileName() const
{
  return ( RR_path + "/po/" + QRMainWin::language() + "/about.txt" ).c_str();
}


QString QAbout::errorMessage() const
{
  return QString( "\n\n\n\n" ) + tr( "Cannot find about.txt file" ) + "\n\n\n"
    + tr( "check config and ROLLDIR environment variable" );
}


QString QAbout::musicFileName() const
{
  struct stat   buf;
  string musicfile = RR_path + "/data/music.adc";
  if( stat( musicfile.c_str(), &buf ) )
    musicfile = RR_path + "/data/music.wav";
  return musicfile.c_str();
}


QString QAbout::temporaryMusicFileName() const
{
  return "/tmp/qroll_music.wav";
}


