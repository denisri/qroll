/***************************************************************************
                          playerServer.cc  -  description
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


#include <roll/game/playerServer.h>
#include <roll/game/game.h>
#include <set>
#include <iostream>

using namespace roll;
using namespace std;


PlayerServer::PlayerServer() : _mode( Local )
{
  //cout << "PlayerServer::PlayerServer\n";
  if( server )
    delete server;
  server = this;
}


PlayerServer::~PlayerServer()
{
  server = 0;
}


unsigned PlayerServer::freeSlot() const
{
  const_iterator	ip, ep=end();
  unsigned		i = 0;
  for( ip=begin(); ip!=ep && (*ip).first == i; ++ip, ++i );
  return( i );
}


unsigned PlayerServer::newPlayer()
{
  unsigned	num = freeSlot();
  addPlayer( num );
  return( num );
}


void PlayerServer::addPlayer( unsigned num )
{
  (*this)[ num ];	// insert

  cout << "addPlayer " << num << endl;

  _bufN[ num ] = 0;
  _buffer[ num ].clear();
  _buffer[ num ].push_back( 0 );
  _buffer[ num ].clear();
  _buffer[ num ].push_back( 0 );
  _sBuffer[ num ].clear();
  _sBuffer[ num ].push_back( false );
  _sBuffer[ num ].push_back( false );
  _repeat[ num ] = false;
}


void PlayerServer::removePlayer( unsigned num )
{
  cout << "removePlayer " << num << endl;
  _keys.erase( num );
  _kbd.erase( num );
  _buffer.erase( num );
  _sBuffer.erase( num );
  _bufN.erase( num );
  _repeatKey.erase( num );
  _repeat.erase( num );
  erase( num );
}


void PlayerServer::makeLocal()
{
  _mode = Local;
  iterator	i, e = end();
  set<unsigned>	todel;

  for( i=begin(); i!=e; ++i )
    if( (*i).second.network != 0 )	// remote player
      todel.insert( (*i).first );

  cout << "PlayerServer::makeLocal, removing " << todel.size() << " players\n";
  cout << "size: " << size() << endl;
  set<unsigned>::iterator	is, es = todel.end();
  for( is=todel.begin(); is!=es; ++is )
    removePlayer( *is );
  cout << "now size: " << size() << endl;
}


void PlayerServer::makeClient( const string &, unsigned short )
{
  PlayerServer::makeLocal();
  _mode = Client;
}


void PlayerServer::makeServer( unsigned short )
{
  PlayerServer::makeLocal();
  _mode = Server;
}


PlayerServer::Mode PlayerServer::mode() const
{
  return( _mode );
}


void PlayerServer::keyPressed( unsigned player, unsigned key )
{
  if( find( player ) == end() )
    {
      cerr << "BUG : keyPressed for inexistant player " << player << endl;
      return;
    }

  map<unsigned, bool>	& kbd = _kbd[ player ];
  vector<unsigned>	& buf = _buffer[ player ];
  vector<bool>		& sbf = _sBuffer[ player ];
  unsigned		& bufN = _bufN[ player ];
  unsigned		& reptK = _repeatKey[ player ];

  kbd[ key ] = true;
  // cout << "player " << player << " pressed key " << key << endl;

  if( key != Game::K_FIRE && ( bufN == 0 || key != buf[0] ) )
    {
      buf[ bufN ] = key;
      sbf[ bufN ] = kbd[ Game::K_FIRE ];
      if( bufN == 0 )
	bufN = 1;
      reptK = key;
      _repeat[ player ] = false;
    }

  if( mode() == Client )
  {
    emitKey( player, true, key );
  }
}


void PlayerServer::keyReleased( unsigned player, unsigned key )
{
  if( find( player ) == end() )
    {
      cerr << "BUG : keyReleased for inexistant player " << player << endl;
      return;
    }

  map<unsigned, bool>	& kbd = _kbd[ player ];
  unsigned		& reptK = _repeatKey[ player ];

  //cout << "joueur " << player << " relache touche " << key << endl;
  if( reptK == key )
    {
      reptK = 0;
      _repeat[ player ] = false;
    }

  kbd[ key ] = false;

  if( mode() == Client )
    emitKey( player, false, key );
}


void PlayerServer::takeKeys()
{
  iterator	ip, fp = end();
  unsigned	player;

  // cout << "PlayerServer::takeKeys()\n";

  if( mode() != Client )
    {
      // cout << "takeKeys local, players: " << size() << endl;
      for( ip = begin(); ip!=fp; ++ip )
	{
	  player = ip->first;
	  // cout << "takeKeys, player " << player << endl;

	  map<unsigned, bool>	& kbd = _kbd[ player ];
	  vector<unsigned>	& buf = _buffer[ player ];
	  vector<bool>		& sbf = _sBuffer[ player ];
	  unsigned		& bufN = _bufN[ player ];
	  unsigned		& reptK = _repeatKey[ player ];
	  bool			& rept = _repeat[ player ];

	  Player		& play = ip->second;

	  if( buf[0] )	//	Buffer clavier
	    {
	      //cout << "buffer\n";
	      play.kbd = buf[0];
	      play.fire = sbf[0];
	      buf[0] = buf[1];
	      sbf[0] = sbf[1];
	      buf[1] = 0;
	      sbf[1] = false;
	      bufN = 0;
	    }
	  else if( reptK )	//	repeating key
	    {
	      //cout << "repeat\n";
	      play.kbd = reptK;
	      play.fire = kbd[ Game::K_FIRE ];
	      // buf[0] = reptK;
	      //sbf[0] = kbd[ Game::K_FIRE ];
	      rept = true;
	    }
	  else		//	Pas de touche bufferisÃ©e
	    {
	      //cout << "nobuf\n";
	      play.fire = kbd[ Game::K_FIRE ];
	      if( kbd[ Game::K_LEFT ] ) 
		play.kbd = Game::K_LEFT;
	      else if( kbd[ Game::K_RIGHT ] ) 
		play.kbd = Game::K_RIGHT;
	      else if( kbd[ Game::K_UP ] ) 
		play.kbd = Game::K_UP;
	      else if( kbd[ Game::K_DOWN ] ) 
		play.kbd = Game::K_DOWN;
	      else if( kbd[ Game::K_BOMB ] ) 
		play.kbd = Game::K_BOMB;
	      else if( kbd[ Game::K_ABORT ] ) 
		play.kbd = Game::K_ABORT;
	      else play.kbd = Game::K_NONE;
	    }
	  // should be removed ?
	  _keys[ player ] = Key( play.kbd, play.fire );
	}
    }

  endTurn();
}


void PlayerServer::flushKbd()
{
}


set<unsigned> PlayerServer::removeNetLink( unsigned num )
{
  set<unsigned>	todel;

  if( num == 0 )
    {
      cerr << "PlayerServer::removeNetLink - can't remove link 0 (local)\n";
      return( todel );
    }

  iterator	i, e = end();

  for( i=begin(); i!=e; ++i )
    if( (*i).second.network == num )	// remote player
      todel.insert( (*i).first );

  cout << "PlayerServer::removeNetLink " << num << ", " << todel.size() << " players\n";
  cout << "server size: " << size() << endl;
  set<unsigned>::iterator	is, es = todel.end();
  for( is=todel.begin(); is!=es; ++is )
    removePlayer( *is );

  cout << "now size: " << size() << endl;
  return todel;
}


void PlayerServer::startGame( unsigned, unsigned, const string & )
{
}


void PlayerServer::stopGame()
{
}


void PlayerServer::pauseGame( bool )
{
}


void PlayerServer::endTurn()
{
}


void PlayerServer::emitKey( unsigned, bool, unsigned )
{
}

