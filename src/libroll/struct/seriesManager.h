/***************************************************************************
                          seriesManager.h  -  description
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


#ifndef ROLL_STRUCT_SERIESMANAGER_H
#define ROLL_STRUCT_SERIESMANAGER_H

#include <string>


namespace roll
{

  ///	Manages loading of series of any format
  class SeriesManager
  {
  public:
    static bool load( const std::string & filename );
  };

}


#endif


