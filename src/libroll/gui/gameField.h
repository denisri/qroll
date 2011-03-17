/***************************************************************************
                          gameField.h  -  description
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


#ifndef ROLL_GUI_GAMEFIELD_H
#define ROLL_GUI_GAMEFIELD_H


/**	Classe de base pour l'affichage. La classe de base ne fait en fait 
	pas de visu, elle gère l'affichage du tableau (sprite par sprite, 
	affichage entier et demi-affichage), le champ de vue dans le tableau, 
	le déplacement du champ de vue etc.
	L'affichage lui-même doit être pris en charge par une classe dérivée 
	spécialisée dans un système graphique (Qt, Motif, Windows, Mac, etc.), 
	qui doit définir les méthodes de dessin en redéfinissant les fonctions 
	virtuelles pures.
 */
class RGameField
{
public:
  enum ScalingMode
  {
    StandardFOV,
    ArbitraryScale,
  };

  RGameField();
  virtual ~RGameField();

  void setPlayer( unsigned p ) { _player = p; }
  unsigned player() const { return( _player ); }
  ///	Déplace le champ de vision vers le haut
  void up();
  ///	Déplace le champ de vision vers le bas
  void down();
  ///	Déplace le champ de vision vers la gauche
  void left();
  ///	Déplace le champ de vision vers la droite
  void right();
  ///	Nombre de sprites affichés en largeur
  unsigned sWidth() const { return( _w ); }
  ///	Num. of \e useful sprites displayed in width (entirely on screen)
  unsigned uWidth() const;
  ///	Nombre de sprites affichés en hauteur
  unsigned sHeight() const { return( _h ); }
  unsigned uHeight() const;
  void enableScaling( bool f ) { if( _scale != f ) { _scale = f; reset(); } }
  bool scalingEnabled() const { return( _scale ); }
  float scaleFactor() const { return( _scfac ); }
  void setScaleFactor( float s ) { _scfac = s; }
  void setScalingMode( ScalingMode m ) { _sclmode = m; }
  ScalingMode scalingMode() const { return _sclmode; }
  unsigned beginX() const { return( _xx ); }
  unsigned beginY() const { return( _yy ); }
  ///	Dessine le tableau, cases entières
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
  ///	Prépare l'écran ou le Pixmap dans lequel le tableau doit être dessiné
  virtual void setupScreen( unsigned w, unsigned h ) = 0;
  ///	Dessine le sprite spr à la position (i, j) dans la zone de dessin
  virtual void copySprite( unsigned spr, int posx, int posy ) = 0;
  /**	Affiche effectivement la zone de dessin (swapbuffer ou copie du 
	pixmap de dessin sur l'écran réel)
	@param	eraseBorder	précise si les bords de la zone de dessin 
				(la partie non affichée) doit être effacée 
				ou non
	@param	w,h	taille utile de la fenêtre (couverte de sprites)
  */
  virtual void updateScreen( bool eraseBorder, int w = 0, int h = 0 ) = 0;

private:
  ///	Coordonnées de la fenêtre de visu dans le tableau: x
  unsigned		_xx;
  ///	Coordonnées de la fenêtre de visu dans le tableau: y
  unsigned		_yy;
  ///	Joueur qui a le focus
  unsigned		_player;
  bool			_scale;
  float			_scfac;
  ///	Nombre de sprites affichés en largeur
  unsigned		_w;
  ///	Nombre de sprites affichés en hauteur
  unsigned		_h;
  ScalingMode           _sclmode;

};



#endif



