/***************************************************************************
                          game.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¨re
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


#include <roll/game/game.h>
#include <roll/game/vars.h>
#include <roll/game/playerServer.h>
#include <roll/sound/soundProcessor.h>
#include <roll/struct/series.h>
#include <roll/game/gElem.h>

using namespace roll;
using namespace std;


Game::Game() 
  : running( false ), paused( false ), changed( false ), 
    allRollsMustExit( false )
{
}


Game::~Game()
{
}


void Game::init( unsigned tab )
{
  //	arrÃªter tous les sons en cours
  RRSoundProcessor::processor().stop();

  if( tab>=ser->numLevels() )
    {
      out << "Game::init : invalid level " << tb << endl;
      fin = true;
      return;
    }

  tb = tab;

  //tempo = 0;
  mec = false;
  twon = 0;
  fin = false;

  ser->unpack( tab, tbct );
  //tbct.make( *ser, tab );
  changed = true;
  lifeGained = 0;
}


void Game::run( unsigned )
{
  tbct.resetScores();
  running = true;
  paused = false;
}


void Game::step()
{
  ++anim;
  changed = false;

  if( fin ) running = false;
  if( fin || !running || paused ) return;

  unsigned				i, nob;
  map<unsigned, bool>			mor;
  PlayerServer::iterator	ip, fp=tbct.players->end();


  //	Vie gagnÃ©e au coup d'avant
  if( lifeGained != 0 )
    lifeGained = 0;

  timeChanged = false;

  //	Comptages

  for( ip=tbct.players->begin(); ip!=fp; ++ip )
    {
      mor[(*ip).first] = (*ip).second.dead;
      //	par dÃ©faut les gens sont morts, jusqu'Ã  ce qu'on les trouve 
      //	en balayant le tableau
      (*ip).second.dead = true;
    }

  for( i=0; i<24; i++ )
    {
      bebet[i].blk = true;
      bebet[i].cpt = false;
    }
  if( tbct.explodeDyna )
    --tbct.explodeDyna;

  //	Boucle du tableau
  //	(elle a une tete un peu plus simple que dans la version 
  //	 Assembleur 68000 pour Atari...)

  unsigned	x, y;

  for( y=0; y<tbct.sizeY(); y++ )
    for( x=0; x<tbct.sizeX(); x++ )
      tbct.d[x][y]->process( x, y );

  //	Gestion des bÃ©bettes

  for( nob=0; nob<24; ++nob )
    if( bebet[nob].cpt != 0 )
      {
	if( bebet[nob].cpt > bebet[nob].tmax )
	  {
	    if( bebet[nob].exi == 0 )
	      bebet[nob].flg = 3;
	    else
	      if( bebet[nob].flg != 3 )
		bebet[nob].flg = 1;
	  }
	bebet[nob].exi = 1;
	if ( bebet[nob].blk == 1 && bebet[nob].flg != 3 )
	  bebet[nob].flg = 2;
      }

  //	Tests gagnÃ© / perdu

  bool	win =false;
  bool	nowin = false;
  for( ip=tbct.players->begin(); ip!=fp; ++ip )
    {
      if( (*ip).second.win )
	win = true;	// au moins 1 gagnant
      if( !(*ip).second.win && !(*ip).second.dead )
	nowin = true;	// au moins 1 non-gagnant encore en jeu
    }
  if( win && ( !allRollsMustExit || !nowin ) )
    {		//	gagnÃ©
      ++twon;
      if( twon == 10 )
	{
	  if( tb < (int) ser->numLevels() )
	    {
	      twon = 0;
	      init( tb+1 );
	      return;
	    }
	  else
	    {
	      fin = true;
	    }
	}
      return;
    }

  for( ip=tbct.players->begin(); ip!=fp; ++ip )
    if( mor[(*ip).first] < (*ip).second.dead && (*ip).second.lives > 0 )
      --(*ip).second.lives;

  bool	lost = true;

  if( !tbct.frozen )
    for( ip=tbct.players->begin(); ip!=fp; ++ip )
      if( !(*ip).second.dead )
	lost = false;

  if( lost )
    {		//	perdu
      ++twon;
      if( twon == 30 )
	{
	  unsigned lives = 0;

	  for( ip=tbct.players->begin(); ip!=fp; ++ip )
	    lives += (*ip).second.lives;

	  if( lives >0 )
	    {
	      twon = 0;
	      init( tb );
	      return;
	    }
	  else
	    {
	      fin = true;
	    }
	}
    }
}











