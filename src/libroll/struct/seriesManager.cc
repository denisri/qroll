/***************************************************************************
                          seriesManager.cc  -  description
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

#include <roll/struct/seriesManager.h>
#include <roll/struct/seriesAtari.h>
#include <roll/struct/seriesRaw.h>
#include <roll/game/vars.h>

using namespace roll;
using namespace std;

//#define ROLL_ATARI_FORMAT_ONLY

bool SeriesManager::load( const string & filename )
{
#ifndef ROLL_ATARI_FORMAT_ONLY
  SeriesRaw	*sr = new SeriesRaw;
  if( sr->load( filename ) )
    {
      delete ser;
      ser = sr;
      return( true );
    }
  else
    delete sr;
#endif

  SeriesAtari	*sa = new SeriesAtari;
  if( sa->load( filename ) )
    {
      delete ser;
      ser = sa;
      return( true );
    }
  else
    delete sa;

  // fail
  return( false );
}


