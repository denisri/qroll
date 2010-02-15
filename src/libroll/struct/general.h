/***************************************************************************
                          general.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2006 by Denis Riviï¿½re
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


#ifndef ROLL_STRUCT_GENERAL_H
#define ROLL_STRUCT_GENERAL_H


#include <iostream>
#include <string>


namespace roll
{

  ///	Sortie d'erreur (compat. Windows: pas de cout)
  extern std::ostream & out;
  extern std::ostream & err;

  void setArgv0( const char* argv0 );
  std::string argv0();
  std::string qRollSharePath();
  std::string rrVersionString();

}


#endif



