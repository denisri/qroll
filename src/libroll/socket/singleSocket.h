/***************************************************************************
                          singleSocket.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2000-2001 by Denis Rivi�e
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

#ifndef ROLL_SOCKET_SINGLESOCKET_H
#define ROLL_SOCKET_SINGLESOCKET_H

#include <qglobal.h>
#if QT_VERSION >= 0x040000
#include <q3socket.h>
typedef Q3Socket QSocket;
#else
#include <qsocket.h>
#endif


namespace roll
{
  class ServerSocket;
  struct SockDescr;

  /// Private class, should not be used directly
  class SingleSocket : public QSocket
  {
    Q_OBJECT

  public:
    SingleSocket( ServerSocket * parent, 
		  const char * name = 0 );
    virtual ~SingleSocket();

    SockDescr* sockDescr() { return( _sd ); }
    void setSockDescr( SockDescr *sd ) { _sd = sd; }

  signals:
    void connectionClosed( SingleSocket* );
    void delayedCloseFinished( SingleSocket* );
    void error( SingleSocket*, int );
    void readyRead( SingleSocket* );
    void bytesWritten( SingleSocket*, int nbytes );

  protected slots:
    void sendConnectionClosed();
    void sendDelayedCloseFinished();
    void sendError( int );
    void sendReadyRead();
    void sendBytesWritten( int );

  protected:

  private:
    ServerSocket	*_serv;
    SockDescr		*_sd;
  };

}


#endif

