/***************************************************************************
                          backFactory.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivi�re
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


#ifndef ROLL_GAME_BACKFACTORY_H
#define ROLL_GAME_BACKFACTORY_H


namespace roll
{
  class RBack;


  /**	Usine � �l�ments d'arri�re-plan */
  class RBackFactory
  {
  public:
    ///
    RBackFactory();
    ///
    ~RBackFactory();

    ///
    RBack* createBack( unsigned num );

  protected:

  private:
  };

}

#endif


