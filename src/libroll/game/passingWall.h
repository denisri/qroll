/***************************************************************************
                          passingWall.h  -  description
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


#ifndef ROLL_GAME_PASSINGWALL_H
#define ROLL_GAME_PASSINGWALL_H


#include <roll/game/foregroundElem.h>


namespace roll
{

  /**	Mur qui laisse passer des choses */
  class RPassingWall : public RForegroundElem
  {
  public:
    RPassingWall( unsigned num ) : RForegroundElem( num ), _inside( 0 ) {}
    virtual ~RPassingWall() { delete _inside; }

    virtual bool isSlippery() const { return( _inside != 0 ); }
    virtual bool canBeInBorder() const { return( true ); }

  protected:
    /**@name	Fonctions protégées */
    //@{
    ///
    virtual void realProcess( unsigned x, unsigned y );
    /**	Teste si l'élément peut entrer dans le mur
	@param	gel	élément à tester
    */
    virtual bool testComeIn( GElem *gel );
    /**	Teste si l'élément peut sortir du mur
	@param	x	coordonnées du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual bool testGoOut( unsigned x, unsigned y );
    /**	Effectue la sortie (peut avoir des effets différents selon le mur)
	@param	x	coordonnées du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual void goOut( unsigned x, unsigned y );
    //@}

    /**@name	Données */
    //@{
    ///	Sprite caché dedans le mur
    GElem	*_inside;
    //@}

  private:
  };

}

#endif


