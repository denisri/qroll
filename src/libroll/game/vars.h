/***************************************************************************
                          vars.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivi�re
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


#ifndef ROLL_GAME_VARS_H
#define ROLL_GAME_VARS_H

#include <roll/struct/general.h>
#include <roll/struct/explosions.h>
#include <roll/struct/blobDescr.h>
#include <roll/game/spritFlag.h>
#include <roll/game/game.h>
#include <roll/game/elemFactory.h>
#include <roll/game/backFactory.h>
#include <roll/game/random.h>
#include <string>

namespace roll
{
  class Series;

  ///	Dircetory des donn�es du jeu
  extern std::string		RR_path;
  ///	S�rie de tableaux en m�moire
  extern Series			*ser;
  ///	Table de conversion des sprites
  extern unsigned char		spr_link[];
  ///	Table des explosions initiales (par d�faut)
  extern RExplosions		expli, explo;
  ///	Table des descripteurs de b�bettes, originale
  extern BlobDescr		bebi[];
  ///	Table des descripteurs de b�bettes, copie qui peut changer
  extern BlobDescr		bebet[];
  ///	Capacit�s des objets
  extern SpritFlag		sp_flg[];
  ///	Moteur du jeu
  extern Game			game;
  /// acces via a function (needed on MinGW/Win32)
  Game & theGame();
  ///	Compteur d'animations
  extern unsigned short		anim;
  ///	Cr�ateur d'�l�ments
  extern RElemFactory		elFactory;
  ///	Cr�ateur de fond
  extern RBackFactory		backFactory;

}

#endif

