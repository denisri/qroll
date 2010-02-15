/***************************************************************************
                          qPlayerServer.h  -  description
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


#ifndef ROLL_SOCKET_QPLAYERSERVER_H
#define ROLL_SOCKET_QPLAYERSERVER_H

#include <roll/game/playerServer.h>
#include <qobject.h>


namespace roll
{

  class PlayerServerSocket;
  class NetMessage;
  struct QPlayerServer_Private;
  typedef std::map<unsigned, unsigned> RenumList;


  class QPlayerServer : public QObject, public PlayerServer
  {
    Q_OBJECT

  public:
    QPlayerServer();
    virtual ~QPlayerServer();

    PlayerServerSocket & socket();

    virtual void makeServer( unsigned short port );
    virtual void makeClient( const std::string & address, 
			     unsigned short port );
    virtual void makeLocal();

    virtual void startGame( unsigned level, unsigned rseed, 
			    const std::string & series );
    virtual void stopGame();
    virtual void pauseGame( bool state );
    virtual void endTurn();
    virtual void emitKey( unsigned player, bool press, unsigned key );
    bool waitingToCompleteTurn() const;

  signals:
    void netHostFound();
    void networkConnected();
    void netError( int err );
    void netClientConnected( int );
    void netClientDisconnected( int );
    void connectionClosed();
    void playersChanged();
    void gameStarted( unsigned level, unsigned rseed, 
		      const std::string & series );
    void gameStopped();
    void gamePaused( bool );
    void message( const std::string & );
    /// Client side: received turn end message from server
    void turnEnded();
    /// Server side: al clients are ready, turn end msg has been sent
    void turnEndEmitted();
    void playersRenumed( const roll::RenumList & );

  public slots:
    virtual void hostFound();
    virtual void clientConnected();
    virtual void socketError( int );
    virtual void messageFromServer( const NetMessage & msg );
    virtual void messageFromClient( unsigned client, const NetMessage & msg );
    virtual void clientDisconnected( int );

  protected:
    void serverTextReceived( unsigned, const NetMessage & );
    void serverAssignPlayersReceived( unsigned, const NetMessage & );
    void serverNumPlayersReceived( unsigned, const NetMessage & );
    void serverPauseGame( unsigned, const NetMessage & );
    void serverKeyEvent( unsigned, const NetMessage & );
    void serverReceiveOK( unsigned, const NetMessage & );
    void clientTextReceived( const NetMessage & );
    void clientAssignPlayersReceived( const NetMessage & );
    void clientNumPlayersReceived( const NetMessage & );
    void clientStartGame( const NetMessage & );
    void clientStopGame( const NetMessage & );
    void clientPauseGame( const NetMessage & );
    void clientKeyEvent( const NetMessage & );
    void clientEndTurn( const NetMessage & );
    void emitEndTurn();

    PlayerServerSocket		*_socket;

  private:
    QPlayerServer_Private	*d;
  };

}


#endif

