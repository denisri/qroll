/***************************************************************************
                          playerServerSocket.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¯Â¿Â½e
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


#include <roll/socket/playerServerSocket.h>
#ifndef ANDROID
#include <roll/socket/qPlayerServer.h>
#include <roll/socket/singleSocket.h>
#endif
#include <roll/socket/netMessages.h>
#include <iostream>
#include <set>


using namespace roll;
using namespace std;


namespace roll
{
  struct PlayerServerSocket_Private
  {
    PlayerServerSocket_Private() : pendingmsg( NetMessage::NONE, 0 ) {}

    NetMessage::MsgHdr	pendingmsg;
    QPlayerServer	*qps;
  };

}


PlayerServerSocket::PlayerServerSocket( QPlayerServer* parent )
  : d( new PlayerServerSocket_Private )
{
  d->qps = parent;
}


PlayerServerSocket::~PlayerServerSocket()
{
  delete d;
}


QPlayerServer* PlayerServerSocket::playerServer()
{
  return( d->qps );
}


const QPlayerServer* PlayerServerSocket::playerServer() const
{
  return( d->qps );
}


#ifndef ANDROID
NetMessage* PlayerServerSocket::readMessage( QTcpSocket* s ) const
{
  NetMessage::MsgHdr	& hdr = d->pendingmsg;

  if( hdr.code == NetMessage::NONE )	// new message
    {
      unsigned		n = s->bytesAvailable();
      const unsigned	hs = sizeof( NetMessage::MsgHdr );

      if( n >= hs )
	{
	  s->read( (char *) &hdr, hs );
	  hdr.msgLen = native( hdr.msgLen );

	  /*cout << "Message, code : " << (int) hdr.code << ", len : " 
	    << hdr.msgLen << endl;*/
	}
      else
	{
	  cout << "Buffer size too small to read message header - delaying\n";
	  return( 0 );
	}
    }
  else
    cout << "attempting to retreive pending message\n";

  if( (unsigned) s->bytesAvailable() < hdr.msgLen )
    {
      cout << "Message longer than pending socket buffer - delaying\n";
      return( 0 );
    }

  NetMessage	*msg = NetMessage::create( hdr.code );
  unsigned	n = hdr.msgLen;
  hdr.code = NetMessage::NONE;	// clear pending message

  if( !msg || !msg->canRead( n ) )
    {
      cerr << "Message can't be read - size error\n";
      delete msg;
      return( 0 );
    }
  char	*buf = new char[ n + 1 ];
  buf[ n ] = '\0';	// protect strings

  s->read( buf, n );
  if( !msg->read( buf, n ) )
    {
      cerr << "Error reading message\n";
      delete msg;
      delete[] buf;
      return( 0 );
    }

  delete[] buf;

  /*cout << "bytes available after readMessage : " << s->bytesAvailable() 
    << endl;*/

  return( msg );
}


void PlayerServerSocket::writeMessage( QTcpSocket* s, const NetMessage & msg )
{
  //cout << "PlayerServerSocket::writeMessage: to QSocket : " << s << endl;
  unsigned long		len = msg.size();
  NetMessage::MsgHdr	hdr( msg.code(), little_endian( len ) );
  s->write( (const char *) &hdr, sizeof( hdr ) );
  char	*wm = msg.write();
  s->write( wm, len );
}
#endif


void PlayerServerSocket::writeMessage( const NetMessage & )
{
}


// ----------------- server -----------------


namespace roll
{

#ifndef ANDROID
  struct SockDescr
  {
    SockDescr( SingleSocket* ss, unsigned n ) : qs( ss ), num( n )
    { ss->setSockDescr( this ); }

    SingleSocket	*qs;
    // computer number on the network
    unsigned		num;
  };

  struct ServerSocket_Private
  {
    ServerSocket_Private( ServerSocket* serv );
    ~ServerSocket_Private();
    SockDescr* sockDescr( unsigned num );
    unsigned	freeNum() const;

    ServerSocket	*server;
    set<SockDescr *>	sockets;
  };
#endif

}


#ifndef ANDROID
ServerSocket_Private::ServerSocket_Private( ServerSocket* serv )
  : server( serv )
{
}


ServerSocket_Private::~ServerSocket_Private()
{
  set<SockDescr *>::iterator	is, es = sockets.end();
  for( is=sockets.begin(); is!=es; ++is )
    delete *is;
}


SockDescr* ServerSocket_Private::sockDescr( unsigned num )
{
  set<SockDescr *>::iterator	is, es = sockets.end();
  for( is=sockets.begin(); is!=es && (*is)->num!=num; ++is );
  if( is == es )
    return( 0 );
  return( *is );
}


unsigned ServerSocket_Private::freeNum() const
{
  set<SockDescr *>::const_iterator	is, es = sockets.end();
  unsigned				num = 1;
  for( is=sockets.begin(); is!=es && (*is)->num==num; ++is, ++num );
  return( num );
}


// **


ServerSocket::ServerSocket( QPlayerServer * parent, quint16 port, 
			    int backlog, const char * name )
  : QTcpServer( parent ), PlayerServerSocket( parent )
{
  d = new ServerSocket_Private( this );
  setObjectName( name );
  connect( this, SIGNAL( newConnection() ), 
           this, SLOT( newConnectionOpens() ) );
  listen( QHostAddress::Any, port );
}


ServerSocket::~ServerSocket()
{
  //cout << "ServerSocket::~ServerSocket\n";
  delete d;
}


void ServerSocket::newConnectionOpens()
{
  QTcpSocket * socket = nextPendingConnection();
  //cout << "new connection, socket " << socket << endl;
  SingleSocket	*s = new SingleSocket( this, "singleSocket" );
  unsigned	n = d->freeNum();
  //cout << "-> client " << n << endl;
  d->sockets.insert( new SockDescr( s, n ) );
  s->setSocketDescriptor( socket->socketDescriptor() );
  connect( s, SIGNAL( connectionClosed( SingleSocket * ) ), this, 
	   SLOT( clientClosed( SingleSocket * ) ) );
  connect( s, SIGNAL( error( SingleSocket *, int ) ), this, 
	   SLOT( clientError( SingleSocket *, int ) ) );
  connect( s, SIGNAL( readyRead( SingleSocket * ) ), this, 
	   SLOT( clientReadyRead( SingleSocket * ) ) );

  MessageText	mt;
  mt.setText( "Rock'N'Roll" );
  PlayerServerSocket::writeMessage( s, mt );
  emit clientConnected( n );
}


void ServerSocket::clientClosed( SingleSocket* c )
{
  int	num = c->sockDescr()->num;
  //cout << "ServerSocket::clientClosed - client " << num << endl;
  d->sockets.erase( c->sockDescr() );
  delete c->sockDescr();
  delete c;
  emit clientDisconnected( num );
}


void ServerSocket::clientError( SingleSocket* c, int )
{
  /*cout << "clientError " << e << ", client " << c->sockDescr()->num 
    << " - closing it" << endl;*/
  d->sockets.erase( c->sockDescr() );
  delete c->sockDescr();
  delete c;
}


void ServerSocket::clientReadyRead( SingleSocket* c )
{
  /*unsigned	sz = c->size();
  cout << "Message from client " << c->sockDescr()->num << " : " 
    << sz << " bytes available\n";*/

  NetMessage	*msg = 0;

  while( c->bytesAvailable() && ( msg = readMessage( c ) ) )
    {
      //cout << "message received\n";
      emit messageReceived( c->sockDescr()->num, *msg );
      delete msg;
    }
  if( !msg )
    cout << "Message NOT properly received (or not entirely)\n";
}


void ServerSocket::writeMessage( const NetMessage & msg )
{
  set<SockDescr *>::const_iterator	is, es = d->sockets.end();
  for( is=d->sockets.begin(); is!=es; ++is )
    PlayerServerSocket::writeMessage( (*is)->qs, msg );
}


void ServerSocket::writeMessage( int conn, const NetMessage & msg )
{
  SockDescr	*sd = d->sockDescr( conn );
  if( sd )
    PlayerServerSocket::writeMessage( sd->qs, msg );
  else
    cerr << "attempt to send message to unknown client " << conn << endl;
}


void ServerSocket::writeMessageToOthers( int conn, const NetMessage & msg )
{
  set<SockDescr *>::iterator	is, es = d->sockets.end();

  for( is=d->sockets.begin(); is!=es; ++is )
    {
      SockDescr	*sd = *is;
      if( sd->num != (unsigned) conn )
	PlayerServerSocket::writeMessage( sd->qs, msg );
    }
}


set<unsigned> ServerSocket::connections() const
{
  set<unsigned>				c;
  set<SockDescr *>::const_iterator	is, es=d->sockets.end();

  for( is=d->sockets.begin(); is!=es; ++is )
    c.insert( (*is)->num );

  return( c );
}


// ---------------- client ---------------------


ClientSocket::ClientSocket( QPlayerServer * parent, const char * name )
  : QTcpSocket( parent ), PlayerServerSocket( parent )
{
  setObjectName( name );
  connect( this, SIGNAL( readyRead() ), SLOT( read() ) );
}


ClientSocket::~ClientSocket()
{
  //cout << "ClientSocket::~ClientSocket\n";
}


void ClientSocket::read()
{
  NetMessage	*msg = 0;

  while( bytesAvailable() && ( msg = readMessage( this ) ) )
    {
      //cout << "read message " << msg << endl;
      emit messageReceived( *msg );
      delete msg;
    }
  if( !msg )
    cout << "Message NOT properly received (or not entirely)\n";
}


void ClientSocket::writeMessage( const NetMessage & msg )
{
  PlayerServerSocket::writeMessage( this, msg );
}

#endif

