/***************************************************************************
                          passingWall.h  -  description
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
    /**@name	Fonctions prot�g�es */
    //@{
    ///
    virtual void realProcess( unsigned x, unsigned y );
    /**	Teste si l'�l�ment peut entrer dans le mur
	@param	gel	�l�ment � tester
    */
    virtual bool testComeIn( GElem *gel );
    /**	Teste si l'�l�ment peut sortir du mur
	@param	x	coordonn�es du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual bool testGoOut( unsigned x, unsigned y );
    /**	Effectue la sortie (peut avoir des effets diff�rents selon le mur)
	@param	x	coordonn�es du mur
	@param	y	(*PAS* de la case de dessous)
    */
    virtual void goOut( unsigned x, unsigned y );
    //@}

    /**@name	Donn�es */
    //@{
    ///	Sprite cach� dedans le mur
    GElem	*_inside;
    //@}

  private:
  };

}

#endif


