/***************************************************************************
                          keyGen.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivière
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


#ifndef ROLL_GAME_KEYGEN_H
#define ROLL_GAME_KEYGEN_H


#include <roll/game/rockGen.h>


namespace roll
{

  /**	Générateur de clefs */
  class RKeyGen : public RRockGen
  {
  public:
    RKeyGen( unsigned num = 232 ) : RRockGen( num ) {}
    virtual ~RKeyGen() {}

    virtual bool canBeInBorder() const { return( true ); }

  protected:
    ///
    virtual unsigned short chooseElem() const;
  };

}

#endif


