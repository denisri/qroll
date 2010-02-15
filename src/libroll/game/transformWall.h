/***************************************************************************
                          transformWall.h  -  description
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


#ifndef ROLL_GAME_TRANSFORMWALL_H
#define ROLL_GAME_TRANSFORMWALL_H


#include <roll/game/passingWall.h>


namespace roll
{

  /**	Mur transformeur	*/
  class RTransformWall : public RPassingWall
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///	Constructeur
    RTransformWall( unsigned num ) : RPassingWall( num ) {}
    ///
    virtual ~RTransformWall() {}
    //@}

  protected:
    /**@name	Fonctions protégées */
    //@{
    /**	Effectue la sortie (peut avoir des effets différents selon le mur)
	@parm	x	coordonnées du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual void goOut( unsigned x, unsigned y );
    ///	Transforme l'élément avant sa sortie
    virtual void transform();
    //@}

  private:
  };

}

#endif


