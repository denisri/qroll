/***************************************************************************
                          holeWall.h  -  description
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


#ifndef ROLL_GAME_HOLEWALL_H
#define ROLL_GAME_HOLEWALL_H


#include <roll/game/passingWall.h>


namespace roll
{

  /**	Mur bouffeur	*/
  class RHoleWall : public RPassingWall
  {
  public:
    /**@name	Constructeur - destructeur */
    //@{
    ///	Constructeur
    RHoleWall( unsigned num ) : RPassingWall( num ) {}
    ///
    virtual ~RHoleWall() {}
    //@}

  protected:
    /**@name	Fonctions protégées */
    //@{
    /**	Bouffe pûrement et simplement ce qu'il y a dans le mur, 
	sans autre forme de procès
	@param	x	coordonnées du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual bool testGoOut( unsigned x, unsigned y );
    //@}

  private:
  };

}

#endif


