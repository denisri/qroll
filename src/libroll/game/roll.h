/***************************************************************************
                          roll.h  -  description
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


#ifndef ROLL_GAME_ROLL_H
#define ROLL_GAME_ROLL_H


#include <roll/game/gElem.h>


namespace roll
{

  /**	Roll (it's the hero (because "Rock and Roll")) */
  class Roll : public GElem
  {
  public:
    ///
    Roll( unsigned num = 34 );
    ///
    virtual ~Roll();

    /**	Teste la possibilit� d'aller dans la case donn�e. 
	Si c'est possible, effectue son nettoyage ("bouffe").
	@param	x	coordonn�e x de la case o� on veut aller
	@param	y	coordonn�e y de la case o� on veut aller
	@param	pouss	pr�cise si on a le droit de pousser l'objet et dans 
	quel sens (-1: gauche, 0: pousse pas, 1: droite)
	@param	no	num�ro du Roll
    */
    virtual bool poss( unsigned x, unsigned y, int pouss, int no );
    ///	Fixe le no du Roll
    void setNum( unsigned num ) { _num = num; }
    ///
    unsigned num() const { return( _num ); }
    ///	Met un autre Roll sous celui-ci (superpos�)
    void setBackRoll( Roll* r ) { _backRoll = r; }
    ///
    Roll *backRoll() const { return( _backRoll ); }
    ///	Roll qui masque this
    Roll *frontRoll( unsigned x, unsigned y ) const;

    /**@name	Propri�t�s de la classe */
    //@{
    ///
    virtual AnimType animType() const { return( AnimVertical ); }
    ///
    virtual bool canMove() const { return( true ); }
    ///
    virtual bool squashedBy( const GElem* el ) const
    { return( el->canFall() ); }
    ///
    virtual bool isRoll() const { return( true ); }
    ///
    virtual bool isExplosive() const { return( true ); }
    //@}

  protected:
    ///
    virtual void realProcess( unsigned x, unsigned y );
    /**	El�ment � laisser derri�re quand on se barre. \\
	Normalement c'est du vide, ou une bombe qu'on vient de poser
	@param	mec	no de Roll
    */
    virtual GElem* elemBehind( unsigned mec, unsigned x, unsigned y );
    /**	Mouvement du Roll.
	Remplace la case courante par le bon �l�ment (vide, Roll de dessus ou 
	de dessous), puis ex�cute les Rolls suivant ( backRoll() )
    */
    void replaceAndProcessBack( unsigned x, unsigned y );
    ///	No du Roll
    unsigned	_num;
    ///	Autre Roll
    Roll	*_backRoll;
  };

}

#endif


