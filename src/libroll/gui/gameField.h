/***************************************************************************
                          gameField.h  -  description
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


#ifndef ROLL_GUI_GAMEFIELD_H
#define ROLL_GUI_GAMEFIELD_H


/**	Classe de base pour l'affichage. La classe de base ne fait en fait 
	pas de visu, elle g�re l'affichage du tableau (sprite par sprite, 
	affichage entier et demi-affichage), le champ de vue dans le tableau, 
	le d�placement du champ de vue etc.
	L'affichage lui-m�me doit �tre pris en charge par une classe d�riv�e 
	sp�cialis�e dans un syst�me graphique (Qt, Motif, Windows, Mac, etc.), 
	qui doit d�finir les m�thodes de dessin en red�finissant les fonctions 
	virtuelles pures.
 */
class RGameField
{
public:
  RGameField();
  virtual ~RGameField();

  void setPlayer( unsigned p ) { _player = p; }
  unsigned player() const { return( _player ); }
  ///	D�place le champ de vision vers le haut
  void up();
  ///	D�place le champ de vision vers le bas
  void down();
  ///	D�place le champ de vision vers la gauche
  void left();
  ///	D�place le champ de vision vers la droite
  void right();
  ///	Nombre de sprites affich�s en largeur
  unsigned sWidth() const { return( _w ); }
  ///	Num. of \e useful sprites displayed in width (entirely on screen)
  unsigned uWidth() const;
  ///	Nombre de sprites affich�s en hauteur
  unsigned sHeight() const { return( _h ); }
  unsigned uHeight() const;
  void enableScaling( bool f ) { if( _scale != f ) { _scale = f; reset(); } }
  bool scalingEnabled() const { return( _scale ); }
  float scaleFactor() const { return( _scfac ); }
  unsigned beginX() const { return( _xx ); }
  unsigned beginY() const { return( _yy ); }
  ///	Dessine le tableau, cases enti�res
  virtual void displayFull();
  ///	Dessine le demi-scroll
  void displayHalf();
  void reset();
  ///	X position (in game elements) of the view origin in the level data
  unsigned posX() const { return( _xx ); }
  ///	Y position (in game elements) of the view origin in the level data
  unsigned posY() const { return( _yy ); }
  void setPos( unsigned x, unsigned y );

  virtual int width() const = 0;
  virtual int height() const = 0;

protected:
  ///	Pr�pare l'�cran ou le Pixmap dans lequel le tableau doit �tre dessin�
  virtual void setupScreen( unsigned w, unsigned h ) = 0;
  ///	Dessine le sprite spr � la position (i, j) dans la zone de dessin
  virtual void copySprite( unsigned spr, int posx, int posy ) = 0;
  /**	Affiche effectivement la zone de dessin (swapbuffer ou copie du 
	pixmap de dessin sur l'�cran r�el)
	@param	eraseBorder	pr�cise si les bords de la zone de dessin 
				(la partie non affich�e) doit �tre effac�e 
				ou non
	@param	w,h	taille utile de la fen�tre (couverte de sprites)
  */
  virtual void updateScreen( bool eraseBorder, int w = 0, int h = 0 ) = 0;

private:
  ///	Coordonn�es de la fen�tre de visu dans le tableau: x
  unsigned		_xx;
  ///	Coordonn�es de la fen�tre de visu dans le tableau: y
  unsigned		_yy;
  ///	Joueur qui a le focus
  unsigned		_player;
  bool			_scale;
  float			_scfac;
  ///	Nombre de sprites affich�s en largeur
  unsigned		_w;
  ///	Nombre de sprites affich�s en hauteur
  unsigned		_h;

};



#endif



