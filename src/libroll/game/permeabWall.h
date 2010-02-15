/***************************************************************************
                          permeabWall.h  -  description
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


#ifndef ROLL_GAME_PERMEABWALL_H
#define ROLL_GAME_PERMEABWALL_H


#include <roll/game/passingWall.h>


namespace roll
{

  /**	Mur perm�able	*/
  class RPermeabWall : public RPassingWall
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///	Constructeur
    RPermeabWall( unsigned num ) : RPassingWall( num ) {}
    ///
    virtual ~RPermeabWall() {}
    //@}

  protected:
    /**@name	Fonctions prot�g�es */
    //@{
    /**	Teste si l'�l�ment peut entrer dans le mur
	@param	gel	�l�ment � tester
    */
    virtual bool testComeIn( GElem *gel );
    //@}

  private:
  };

}

#endif


