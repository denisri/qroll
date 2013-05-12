/***************************************************************************
                          playerServerSocket.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Riviï¿½e
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


#ifndef ROLL_SOCKET_PLAYERSERVERSOCKET_H
#define ROLL_SOCKET_PLAYERSERVERSOCKET_H

#include <qglobal.h>
#ifndef ANDROID
#include <QTcpSocket>
#include <QTcpServer>
#endif
#include <set>


namespace roll
{
  struct ServerSocket_Private;
  class SingleSocket;
  class NetMessage;
  struct PlayerServerSocket_Private;
  class QPlayerServer;


  class PlayerServerSocket
  {
  public:
    PlayerServerSocket( QPlayerServer* parent );
    virtual ~PlayerServerSocket();

#ifndef ANDROID
    virtual NetMessage* readMessage( QTcpSocket* s ) const;
    virtual void writeMessage( QTcpSocket* s, const NetMessage & msg );
#endif
    virtual void writeMessage( const NetMessage & msg );
    QPlayerServer* playerServer();
    const QPlayerServer* playerServer() const;

  private:
    PlayerServerSocket_Private	*d;
  };


#ifndef ANDROID
  class ServerSocket : public QTcpServer, public PlayerServerSocket
  {
    Q_OBJECT

  public:
    ServerSocket( QPlayerServer * parent, quint16 port, int backlog = 0, 
		  const char * name = 0 );
    virtual ~ServerSocket();

    virtual void writeMessage( const NetMessage & msg );
    virtual void writeMessage( int connec, const NetMessage & msg );
    /// send message to all but \a connec
    virtual void writeMessageToOthers( int connec, const NetMessage & msg );
    /// connection numbers list
    std::set<unsigned> connections() const;

  signals:
    void error( int );
    void connected( int );
    void messageReceived( unsigned client, const NetMessage & msg );
    void clientConnected( int );
    void clientDisconnected( int );

  public slots:
    void clientClosed( SingleSocket* );
    void clientError( SingleSocket*, int );
    void clientReadyRead( SingleSocket* );
    void newConnectionOpens();

  private:
    ServerSocket_Private	*d;
  };


  class ClientSocket : public QTcpSocket, public PlayerServerSocket
  {
    Q_OBJECT

  public:
    ClientSocket( QPlayerServer* parent, const char * name = 0 );
    virtual ~ClientSocket();

    virtual void writeMessage( const NetMessage & msg );

  signals:
    void messageReceived( const NetMessage & msg );

  protected slots:
    void read();

  protected:
  };
#endif

  //	inline utility functions

  ///	Converts a 32 bits unsigned to big_endian
  inline unsigned big_endian( unsigned long n )
  {
    unsigned char	nn[4];
    nn[0] = n >> 24;
    nn[1] = ( n >> 16 ) & 0xff;
    nn[2] = ( n >> 8 ) & 0xff;
    nn[3] = n & 0xff;
    return( *(unsigned *) nn );
  }

  ///	Converts a 32 bits unsigned to little_endian
  inline unsigned little_endian( unsigned n )
  {
    unsigned char	nn[4];
    nn[0] = n & 0xff;
    nn[1] = ( n >> 8 ) & 0xff;
    nn[2] = ( n >> 16 ) & 0xff;
    nn[3] = n >> 24;
    return( *(unsigned *) nn );
  }

  ///	Converts a 16 bits unsigned to little_endian
  inline unsigned little_endian( unsigned short n )
  {
    unsigned char	nn[2];
    nn[0] = n & 0xff;
    nn[1] = n >> 8;
    return( *(unsigned short *) nn );
  }


  ///	Converts a little_endian 32 bits unsigned to native format
  inline unsigned native( unsigned n )
  {
    unsigned char	*nn = (unsigned char *) &n;
    return( (nn[3] << 24) | (nn[2] << 16) | (nn[1] << 8) | nn[0] );
  }

  ///	Converts a little_endian 16 bits unsigned to native format
  inline unsigned native( unsigned short n )
  {
    unsigned char	*nn = (unsigned char *) &n;
    return( (nn[1] << 8) | nn[0] );
  }

}


#endif

