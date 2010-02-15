/***************************************************************************
                          netMessages.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2001 by Denis Rivière
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

#ifndef ROLL_SOCKET_NETMESSAGES_H
#define ROLL_SOCKET_NETMESSAGES_H

#include <roll/game/playerServer.h>
#include <string>
#include <vector>


namespace roll
{

  /**	Base class for all network messages. Provides a common way for 
	exchanging information using the read() and write() methods. 
	The base NetMessage class also contains a factory used to create the 
	right message object from its code before reading its internals */
  class NetMessage
  {
  public:
    enum MessageId
    {
      /// No message (empty header)
      NONE, 
      /// Start a new game (server->clients)
      START_GAME, 
      /// Unexpected / misunderstood message received
      CONFUSED, 
      /// Confirm message correctly received and decoded
      RECEIVE_OK, 
      /// Describes all the players handled on one side of the connection
      PLAYERS, 
      /// A new players has joined
      ADD_PLAYER, 
      /// A player has left the game
      REMOVE_PLAYER, 
      /// Game stops
      STOP_GAME, 
      /// Go to next game turn (server->clients)
      NEXT_STEP, 
      /// Text message (debugging)
      TEXT, 
      /// Tell how many players are on this side of the network
      NUMPLAYERS, 
      /// Assign players number (server to client)
      ASSIGNPLAYERS, 
      PAUSE_GAME, 
      KEYEVENT, 
      END_TURN
    };

    ///	Message Header
    struct MsgHdr
    {
      MsgHdr( unsigned char c, unsigned long l ) : code( c ), msgLen( l ) {}
      ///	Message type code
      unsigned char	code;
      ///	Size of message, little endian
      unsigned long	msgLen;
    };

    /// Factory function
    static NetMessage* create( unsigned char code );

    NetMessage( unsigned char cd );
    virtual ~NetMessage();

    /// size in bytes of the message contents
    virtual unsigned size() const = 0;
    /// checks if message can be read from the right size buffer
    virtual bool canRead( unsigned len ) const;
    /** reads the message stored in buffer. Checks canRead() before 
	proceding. The default behaviour just memcpy() to this, so 
	be careful... */
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;
    unsigned char code() const { return( _code ); }

    unsigned char	_code;
  };


  ///	Emitted by clients after each turn
  class MessageOK : public NetMessage
  {
  public:
    MessageOK();
    virtual ~MessageOK();

    virtual unsigned size() const;
  };


  /**	Text messages, almost useless but used when connecting: the server 
	and the client both sends a text message saying "Rock'N'Roll" to 
	ensure the connection is OK and they are both talking a a R&R game */
  class MessageText : public NetMessage
  {
  public:
    MessageText();
    virtual ~MessageText();

    virtual unsigned size() const;
    virtual bool canRead( unsigned len ) const;
    virtual bool read( const char* buffer, unsigned len );
    void setText( const std::string & txt );
    virtual char* write() const;

    std::string	text;
  };


  /**	Message used to signal new or leaving players and/or assign them 
	a number. It is used by both the server and clients, but only the 
	server is able to renumber existing players */
  class MessageAssignPlayers : public NetMessage
  {
  public:
    MessageAssignPlayers();
    virtual ~MessageAssignPlayers();

    virtual unsigned size() const;
    virtual bool canRead( unsigned len ) const;
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;

    /**	whose players are described here ?
	0: server, 1: yours (client), 2: new players (other client), 
	3: removed players */
    unsigned		whose;
    std::vector<unsigned>	players;
  };


  /**	This message is emited by clients upon connection to indicate how 
	many players are already present on the client computer. The 
	server will renumber them and send back 2 MessageAssignPlayers 
	messages, one to describe server-side players, and one to give local 
	players the right numbers */
  class MessageNumPlayers : public NetMessage
  {
  public:
    MessageNumPlayers();
    virtual ~MessageNumPlayers();

    virtual unsigned size() const;
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;

    unsigned	num;
  };


  /**	Message emitted to clients when the server starts a new game */
  class MessageStartGame : public NetMessage
  {
  public:
    MessageStartGame();
    virtual ~MessageStartGame();

    virtual unsigned size() const;
    virtual bool canRead( unsigned len ) const;
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;

    unsigned		level;
    unsigned		randomSeed;
    unsigned char	rollExitFlag;
    std::string		series;
  };


  class MessageStopGame : public NetMessage
  {
  public:
    MessageStopGame();
    virtual ~MessageStopGame();

    virtual unsigned size() const;
  };


  class MessagePauseGame : public NetMessage
  {
  public:
    MessagePauseGame();
    virtual ~MessagePauseGame();

    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;
    virtual unsigned size() const;

    unsigned char	state;
  };


  class MessageEndTurn : public NetMessage
  {
  public:
    MessageEndTurn();
    virtual ~MessageEndTurn();

    virtual unsigned size() const;
    virtual bool canRead( unsigned len ) const;
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;

    std::map<unsigned, Key>	keys;
  };


  class MessageKey : public NetMessage
  {
  public:
    MessageKey();
    virtual ~MessageKey();

    virtual unsigned size() const;
    virtual bool read( const char* buffer, unsigned len );
    virtual char* write() const;

    unsigned		player;
    unsigned		key;
    unsigned char	pressRelease;
  };

}


#endif


