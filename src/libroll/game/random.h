/***************************************************************************
                          random.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Rivière
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

#ifndef ROLL_GAME_RANDOM_H
#define ROLL_GAME_RANDOM_H


#include <string>


namespace roll
{
  struct Random_Private;

  class Random
  {
  public:
    Random();
    ~Random();

    /** utility function: random generator. Rock'n'roll uses a wrapper for 
	rand() to ensure all generated numbers are synchronous on a network */
    unsigned char operator () ();
    /// Inits the random generator rrand()
    void srand( unsigned seed );
    /// Loads the (binary) random numbers map
    bool loadTable( const std::string & filename );
    /// Generates a random table using rand()
    void genTable( unsigned length );
    /// saves the current table to disk
    bool saveTable( const std::string & filename );

  private:
    Random_Private	*d;
  };

  ///	Synchronous random generator
  extern Random	rrand;
}


#endif

