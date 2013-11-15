/***************************************************************************
                          vars.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivière
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

  ///	Game data dircetory
  extern std::string		RR_path;
  ///	Levels series in memory
  extern Series			*ser;
  ///	Table de conversion des sprites
  extern unsigned char		spr_link[];
  ///	Table des explosions initiales (default)
  extern RExplosions		expli, explo;
  ///	Blobs descriptors table, (read-only) original
  extern BlobDescr		bebi[];
  ///	Blobs descriptors table, read-write copy
  extern BlobDescr		bebet[];
  ///	Elements capabilities
  extern SpritFlag		sp_flg[];
  ///	Game engine
  extern Game			game;
  /// acces via a function (needed on MinGW/Win32)
  Game & theGame();
  ///	Compteur d'animations
  extern unsigned short		anim;
  ///	Elements generator
  extern RElemFactory		elFactory;
  ///	Backgroud generator
  extern RBackFactory		backFactory;

}

#endif

