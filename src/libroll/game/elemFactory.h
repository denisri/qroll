/***************************************************************************
                          elemFactory.h  -  description
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


#ifndef ROLL_GAME_ELEMFACTORY_H
#define ROLL_GAME_ELEMFACTORY_H


namespace roll
{
  class GElem;

  /**	Usine à éléments de jeu ("sprites") */
  class RElemFactory
  {
  public:
    ///
    RElemFactory();
    ///
    virtual ~RElemFactory();
    ///
    GElem* createElem( unsigned num );

  protected:
  };

}

#endif



