/***************************************************************************
                          blob.h  -  description
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


#ifndef ROLL_GAME_BEBETTE_H
#define ROLL_GAME_BEBETTE_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	B�bette qui grossit */
  class RBlob : public GElem
  {
  public:
    RBlob( unsigned num );
    virtual ~RBlob();

    virtual AnimType animType() const { return( AnimReturn ); }
    virtual bool isBlob() const { return( true ); }
    virtual bool canBeInBorder() const { return( true ); }

  protected:
    virtual void realProcess( unsigned x, unsigned y );
    /**	Teste et effectue le grossissement sur la case donn�e
	@param	x	coord. case � envahir
	@param	y	pareil
	@param	nob	no de b�bette
	@param	flags	flags � mettre � la nouvelle case de b�bette
    */
    virtual bool grow( unsigned x, unsigned y, unsigned nob, 
		       unsigned flags = 0 );

  private:
  };

}

#endif


