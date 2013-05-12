/***************************************************************************
                          singleSocket.h  -  description
                             -------------------
    begin                : 2001
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

#include <roll/socket/singleSocket.h>
#include <roll/socket/playerServerSocket.h>
#include <QTcpSocket>

using namespace roll;
using namespace std;


// -------------- single socket -------------


#ifndef ANDROID
SingleSocket::SingleSocket( ServerSocket* parent, QTcpSocket* socket,
                            const char* name ) 
  : QObject( parent ), _serv( parent ), _socket( socket ), _sd( 0 )
{
  setObjectName( name );
  // socket->setParent( this );
  // redirect signals to variants telling who I am
  connect( socket, SIGNAL( disconnected() ), 
           this, SLOT( sendConnectionClosed() ) );
//   connect( this, SIGNAL( delayedCloseFinished() ), 
//         SLOT( sendDelayedCloseFinished() ) );
  connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ), 
           this, SLOT( sendError( QAbstractSocket::SocketError ) ) );
  connect( socket, SIGNAL( readyRead() ), this, SLOT( sendReadyRead() ) );
  connect( socket, SIGNAL( bytesWritten( qint64 ) ), 
           this, SLOT( sendBytesWritten( qint64 ) ) );
}


SingleSocket::~SingleSocket()
{
  _socket->disconnect();
  _socket->disconnectFromHost();
  // delete _socket;
}


void SingleSocket::sendConnectionClosed()
{
  emit connectionClosed( this );
}


// void SingleSocket::sendDelayedCloseFinished()
// {
//   emit delayedCloseFinished( this );
// }


void SingleSocket::sendError( QAbstractSocket::SocketError e )
{
  emit error( this, (int) e );
}


void SingleSocket::sendReadyRead()
{
  emit readyRead( this );
}


void SingleSocket::sendBytesWritten( qint64 nbytes )
{
  emit bytesWritten( this, (int) nbytes );
}
#endif


