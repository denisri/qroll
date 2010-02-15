/***************************************************************************
                          eatable.h  -  description
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


#ifndef ROLL_GAME_EATABLE_H
#define ROLL_GAME_EATABLE_H


#include <roll/game/gElem.h>


namespace roll
{
  /**	Chose qui se mange */
  class REatable : public GElem
  {
  public:
    ///
    REatable( unsigned num ) : GElem( num ) {}
    ///
    virtual ~REatable();

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isEatable() const { return( true ); }
    //@}
  };

}

#endif

