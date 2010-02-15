/***************************************************************************
                          roll.h  -  description
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

    /**	Teste la possibilité d'aller dans la case donnée. 
	Si c'est possible, effectue son nettoyage ("bouffe").
	@param	x	coordonnée x de la case où on veut aller
	@param	y	coordonnée y de la case où on veut aller
	@param	pouss	précise si on a le droit de pousser l'objet et dans 
	quel sens (-1: gauche, 0: pousse pas, 1: droite)
	@param	no	numéro du Roll
    */
    virtual bool poss( unsigned x, unsigned y, int pouss, int no );
    ///	Fixe le no du Roll
    void setNum( unsigned num ) { _num = num; }
    ///
    unsigned num() const { return( _num ); }
    ///	Met un autre Roll sous celui-ci (superposé)
    void setBackRoll( Roll* r ) { _backRoll = r; }
    ///
    Roll *backRoll() const { return( _backRoll ); }
    ///	Roll qui masque this
    Roll *frontRoll( unsigned x, unsigned y ) const;

    /**@name	Propriétés de la classe */
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
    /**	Elément à laisser derrière quand on se barre. \\
	Normalement c'est du vide, ou une bombe qu'on vient de poser
	@param	mec	no de Roll
    */
    virtual GElem* elemBehind( unsigned mec, unsigned x, unsigned y );
    /**	Mouvement du Roll.
	Remplace la case courante par le bon élément (vide, Roll de dessus ou 
	de dessous), puis exécute les Rolls suivant ( backRoll() )
    */
    void replaceAndProcessBack( unsigned x, unsigned y );
    ///	No du Roll
    unsigned	_num;
    ///	Autre Roll
    Roll	*_backRoll;
  };

}

#endif


