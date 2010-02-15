/***************************************************************************
                          playerServer.h  -  description
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


#ifndef ROLL_GAME_PLAYERSERVER_H
#define ROLL_GAME_PLAYERSERVER_H


#include <roll/game/player.h>
#include <map>
#include <vector>
#include <string>
#include <set>


namespace roll
{

  struct Key
  {
    Key() {}
    Key( unsigned short k, unsigned short f ) : key( k ), fire( f ) {}

    unsigned short key;
    unsigned short fire;
  };

  /**	PlayerServer handles players numbers and states. It's a singleton 
	and its unique instance can be accessed via the static server variable.
	PlayerServer can be inherited and re-instantiated. When a new instance 
	is created, the static server variable is changed and the older 
	instance is deleted */
  class PlayerServer : public std::map<unsigned, Player>
  {
  public:
    enum Mode
    {
      Local, 
      Server, 
      Client
    };

    PlayerServer();
    virtual ~PlayerServer();

    unsigned freeSlot() const;
    Mode mode() const;
    virtual void makeServer( unsigned short port );
    virtual void makeClient( const std::string & address, 
			     unsigned short port );
    virtual void makeLocal();

    virtual unsigned newPlayer();
    virtual void addPlayer( unsigned num );
    virtual void removePlayer( unsigned p );
    /** Deletes all players belonging to network connection \a num
	\r deleted players numbers */
    virtual std::set<unsigned> removeNetLink( unsigned num );

    virtual void keyPressed( unsigned player, unsigned key );
    virtual void keyReleased( unsigned player, unsigned key );
    /**	called at the end of each game turn to synchronize networked players 
	and set keys actually used next turn in the game */
    virtual void takeKeys();
    virtual void flushKbd();
    const std::map<unsigned, Key> & keys() const { return( _keys ); }

    const std::string & address() const { return( _address ); }
    void setAddress( const std::string & a ) { _address = a; }
    unsigned short port() const { return( _port ); }
    void setPort( unsigned short p ) { _port = p; }

    virtual void startGame( unsigned level, unsigned rseed, 
			    const std::string & series );
    virtual void stopGame();
    virtual void pauseGame( bool state );
    /// sends keys and end-turn signal (if server) to clients
    virtual void endTurn();
    /// sends a key press or release event to the server
    virtual void emitKey( unsigned player, bool press, unsigned key );

    static PlayerServer	*server;

  protected:
    Mode						_mode;
    ///	Liste des touches de tous les joueurs
    std::map<unsigned, Key>				_keys;

    ///	Clavier: image des touches enfoncées
    std::map<unsigned, std::map<unsigned, bool> >	_kbd;
    ///	Buffer clavier (2 touches)
    std::map<unsigned, std::vector<unsigned> >		_buffer;
    ///	Buffer des modifieurs (ctrl enfoncée)
    std::map<unsigned, std::vector<bool> >		_sBuffer;
    ///	Index dans buffer
    std::map<unsigned, unsigned>			_bufN;
    ///	Touche en cours de répétition
    std::map<unsigned, unsigned>			_repeatKey;
    ///	Répétition de touche entamée ?
    std::map<unsigned, bool>				_repeat;

    std::string		_address;
    unsigned short	_port;
  };

}


#endif

