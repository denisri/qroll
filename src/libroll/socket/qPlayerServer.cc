/***************************************************************************
                          qPlayerServer.cc  -  description
                             -------------------
    begin                : 1999
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


#include <roll/socket/qPlayerServer.h>
#include <roll/socket/playerServerSocket.h>
#include <roll/socket/netMessages.h>
#include <roll/socket/netMessages.h>
#include <roll/game/vars.h>
#if QT_VERSION >= 0x040000
#include <q3socket.h>
typedef Q3Socket QSocket;
#endif
#ifdef _WIN32
#include <winsock.h>
#else
#include <netdb.h>
#endif

using namespace roll;
using namespace std;


namespace roll
{
  typedef void (QPlayerServer::*ServerMsgFunc) ( unsigned, 
						 const NetMessage & );
  typedef void (QPlayerServer::*ClientMsgFunc) ( const NetMessage & );

  struct QPlayerServer_Private
  {
    QPlayerServer_Private() : waitingToComplete( false ) {}

    map<unsigned char, ServerMsgFunc>	servMsg;
    map<unsigned char, ClientMsgFunc>	clientMsg;
    set<unsigned>			clients;
    set<unsigned>			readyClients;
    bool				waitingToComplete;
  };

}


QPlayerServer::QPlayerServer()
  : QObject( 0 ), PlayerServer(), d( new QPlayerServer_Private )
{
  _socket = new PlayerServerSocket( this );

  d->servMsg[ (unsigned char) NetMessage::TEXT          ] 
    = &QPlayerServer::serverTextReceived;
  d->servMsg[ (unsigned char) NetMessage::NUMPLAYERS    ] 
    = &QPlayerServer::serverNumPlayersReceived;
  d->servMsg[ (unsigned char) NetMessage::ASSIGNPLAYERS ] 
    = &QPlayerServer::serverAssignPlayersReceived;
  d->servMsg[ (unsigned char) NetMessage::PAUSE_GAME    ] 
    = &QPlayerServer::serverPauseGame;
  d->servMsg[ (unsigned char) NetMessage::KEYEVENT      ] 
    = &QPlayerServer::serverKeyEvent;
  d->servMsg[ (unsigned char) NetMessage::RECEIVE_OK    ] 
    = &QPlayerServer::serverReceiveOK;

  d->clientMsg[ (unsigned char) NetMessage::TEXT          ] 
    = &QPlayerServer::clientTextReceived;
  d->clientMsg[ (unsigned char) NetMessage::NUMPLAYERS    ] 
    = &QPlayerServer::clientNumPlayersReceived;
  d->clientMsg[ (unsigned char) NetMessage::ASSIGNPLAYERS ] 
    = &QPlayerServer::clientAssignPlayersReceived;
  d->clientMsg[ (unsigned char) NetMessage::START_GAME    ] 
    = &QPlayerServer::clientStartGame;
  d->clientMsg[ (unsigned char) NetMessage::PAUSE_GAME    ] 
    = &QPlayerServer::clientPauseGame;
  d->clientMsg[ (unsigned char) NetMessage::STOP_GAME     ] 
    = &QPlayerServer::clientStopGame;
  d->clientMsg[ (unsigned char) NetMessage::END_TURN      ] 
    = &QPlayerServer::clientEndTurn;
}


QPlayerServer::~QPlayerServer()
{
  //cout << "QPlayerServer::~QPlayerServer\n";
  delete _socket;
}


PlayerServerSocket & QPlayerServer::socket()
{
  return( *_socket );
}


void QPlayerServer::makeServer( unsigned short port )
{
  //cout << "QPlayerServer::makeServer\n";

  PlayerServer::makeServer( port );
  delete _socket;
  ServerSocket	*sk = new ServerSocket( this, port, 0, "serverSocket" );
  _socket = sk;
  if( !sk->ok() )
    {
      cout << "server couldn't launch !\n";
      makeLocal();
      emit netError( QSocket::ErrConnectionRefused );
      return;
    }
  PlayerServer::makeServer( port );
  cout << "seems OK, waiting for incoming connections\n";
  connect( sk, SIGNAL( error( int ) ), this, SLOT( socketError( int ) ) );
  connect( sk, SIGNAL( messageReceived( unsigned, const NetMessage & ) ), 
	   this, SLOT( messageFromClient( unsigned, const NetMessage & ) ) );
  connect( sk, SIGNAL( clientConnected( int ) ), this, 
	   SIGNAL( netClientConnected( int ) ) );
  connect( sk, SIGNAL( clientDisconnected( int ) ), this, 
	   SLOT( clientDisconnected( int ) ) );
  emit networkConnected();
}


void QPlayerServer::makeClient( const string & address, unsigned short port )
{
  //cout << "QPlayerServer::makeClient\n";

  PlayerServer::makeClient( address, port );
  delete _socket;
  _mode = Local;

  // address lookup
  //cout << "hostname : " << address << endl;

  struct hostent	*hent = gethostbyname( address.c_str() );
  if( hent == NULL )
    {
      cerr << "gethostbyname failed - unable to lookup IP address " 
	   << "for server " << address << ".\n";
      //emit error( QSocket::ErrHostNotFound );
      return;
    }

  ClientSocket	*sk = new ClientSocket( this, "clientSocket" );
  _socket = sk;
  setAddress( address );
  setPort( port );
  connect( sk, SIGNAL( hostFound() ), this, SLOT( hostFound() ) );
  connect( sk, SIGNAL( connected() ), this, SLOT( clientConnected() ) );
  connect( sk, SIGNAL( error( int ) ), this, SLOT( socketError( int ) ) );
  //cout <<  "connectToHost " << hent->h_name << endl;
  sk->connectToHost( hent->h_name, port );
  connect( sk, SIGNAL( messageReceived( const NetMessage & ) ), this, 
	   SLOT( messageFromServer( const NetMessage & ) ) );
  connect( sk, SIGNAL( connectionClosed() ), this, 
	   SIGNAL( connectionClosed() ) );
}


void QPlayerServer::makeLocal()
{
  //cout << "QPlayerServer::makeLocal\n";
  delete _socket;
  _socket = new PlayerServerSocket( this );
  PlayerServer::makeLocal();
}


void QPlayerServer::hostFound()
{
  //cout << "QPlayerServer::hostFound - connecting...\n";
  emit netHostFound();
}


void QPlayerServer::clientConnected()
{
  //cout << "QPlayerServer, host connected - client created\n";
  PlayerServer::makeClient( address(), port() );

  ClientSocket	*sk = (ClientSocket *) _socket;
  //sk->writeBlock( "Hello", 5 );
  MessageText	mt;
  mt.setText( "Rock'N'Roll" );
  sk->writeMessage( mt );
  emit networkConnected();
}


void QPlayerServer::socketError( int err )
{
  cout << "QPlayerServer: socket error " << err << " - returning to local\n";
  makeLocal();
  emit netError( err );
}

// message handling

void QPlayerServer::messageFromClient( unsigned client, 
				       const NetMessage & msg )
{
  //cout << "QPlayerServer::messageFromClient " << client << "\n";

  map<unsigned char,ServerMsgFunc>::const_iterator	ic 
    = d->servMsg.find( msg.code() );
  if( ic == d->servMsg.end() )
    {
      cerr << "Message code " << (int) msg.code() << " unrecognized\n";
      return;
    }
  else
    (this->*(*ic).second)( client, msg );
}


void QPlayerServer::messageFromServer( const NetMessage & msg )
{
  //cout << "QPlayerServer::messageFromServer\n";

  map<unsigned char,ClientMsgFunc>::const_iterator	ic 
    = d->clientMsg.find( msg.code() );
  if( ic == d->clientMsg.end() )
    {
      cerr << "Message code " << (int) msg.code() << " unrecognized\n";
      return;
    }
  else
    (this->*(*ic).second)( msg );
}


void QPlayerServer::clientDisconnected( int num )
{
  set<unsigned>	r = removeNetLink( (unsigned) num );
  if( !r.empty() )
    {
      MessageAssignPlayers	mr;
      mr.whose = 3;	// removed
      mr.players.reserve( r.size() );
      set<unsigned>::iterator	ip, ep = r.end();
      for( ip=r.begin(); ip!=ep; ++ip )
	mr.players.push_back( *ip );
      // broadcast message
      ( (ServerSocket *) _socket )->writeMessageToOthers( num, mr );
    }

  emit netClientDisconnected( num );
}


void QPlayerServer::serverTextReceived( unsigned client, 
					const NetMessage & msg )
{
  const MessageText	&mt = (MessageText &) msg;
  if( mt.text == "Rock'N'Roll" )
    {
      cout << "connection message received\n";
    }
  else
    cout << "Message from computer " << client << " :\n"
	 << mt.text << endl;
}


void QPlayerServer::serverAssignPlayersReceived( unsigned client, 
						 const NetMessage & msg )
{
  ServerSocket	*sk = (ServerSocket *) _socket;

  const MessageAssignPlayers	& m = (MessageAssignPlayers &) msg;
  cout << "AssignPlayers : whose : " << m.whose << endl;
  cout << "                num   : " << m.players.size() << endl;

  unsigned	i, n = m.players.size();

  switch( m.whose )
    {
    case 2:	// new client players: add
      {
	// must renum them before adding
	map<unsigned, unsigned>	np;
	for( i=0; i<n; ++i )
	  np[ m.players[i] ] = newPlayer();

	// notify new assignation to client
	// ...

	// notify new players to other clients
	MessageAssignPlayers	mp;
	mp.whose = 2;
	map<unsigned, unsigned>::iterator	im, em=np.end();
	for( im=np.begin(); im!=em; ++im )
	  mp.players.push_back( (*im).second );
	// broadcast message
	sk->writeMessageToOthers( client, mp );
      }
      break;
    case 3:	// removed players
      for( i=0; i<n; ++i )
	removePlayer( m.players[i] );
      break;
    default:
      cout << "invalid assign mode\n";
      break;
    }
  emit playersChanged();
}


void QPlayerServer::serverNumPlayersReceived( unsigned client, 
					      const NetMessage & msg )
{
  ServerSocket	*sk = (ServerSocket *) _socket;

  const MessageNumPlayers	&m = (MessageNumPlayers &) msg;
  cout << "players already on client : " << m.num << endl;

  MessageAssignPlayers	apm, apy;
  unsigned		i, p;
  iterator		ip, ep=end();

  apm.whose = 0;
  apy.whose = 1;

  for( ip=begin(); ip!=ep; ++ip )
    apm.players.push_back( (*ip).first );

  for( i=0; i<m.num; ++i )
    {
      p = newPlayer();
      Player	& pl = (*this)[ p ];
      pl.network = client;
      apy.players.push_back( p );
    }
  sk->writeMessage( client, apy );
  sk->writeMessage( client, apm );

  apy.whose = 2;	// notify other clients
  sk->writeMessageToOthers( client, apy );
}


void QPlayerServer::serverPauseGame( unsigned client, const NetMessage & msg )
{
  ServerSocket	*s = dynamic_cast<ServerSocket *>( _socket );
  if( s )
    s->writeMessageToOthers( client, msg );

  MessagePauseGame	& m = (MessagePauseGame &) msg;

  //cout << "Pause : " << (int) m.state << endl;
  emit gamePaused( m.state );
}


void QPlayerServer::serverKeyEvent( unsigned, const NetMessage & msg )
{
  //cout << "Key event on client " << client << endl;

  MessageKey	& m = (MessageKey &) msg;

  if( m.pressRelease )
    keyPressed( m.player, m.key );
  else
    keyReleased( m.player, m.key );
}


void QPlayerServer::serverReceiveOK( unsigned client, const NetMessage & )
{
  //cout << "serverReceiveOK\n";
  d->readyClients.insert( client );
  if( d->waitingToComplete && d->readyClients.size() == d->clients.size() )
    emitEndTurn();	// we were waiting for this last one, next turn !
}


void QPlayerServer::clientTextReceived( const NetMessage & msg )
{
  ClientSocket	*sk = (ClientSocket *) _socket;
  const MessageText	&mt = (MessageText &) msg;

  if( mt.text == "Rock'N'Roll" )
    {
      cout << "connection message received\n";
      MessageNumPlayers	mn;
      mn.num = size();
      sk->writeMessage( mn );
    }
  else
    cout << "Message from server :\n"
	 << mt.text << endl;
}


void QPlayerServer::clientAssignPlayersReceived( const NetMessage & msg )
{
  const MessageAssignPlayers	& m = (MessageAssignPlayers &) msg;

  cout << "AssignPlayers : whose : " << m.whose << endl;
  cout << "                num   : " << m.players.size() << endl;

  unsigned	i, n = m.players.size();

  switch( m.whose )
    {
    case 0:	// server
      // remove all non-local players
      // ...
    case 2:	// new server players: add
      for( i=0; i<n; ++i )
	{
	  addPlayer( m.players[i] );
	  (*this)[ m.players[i] ].network = 1;
	}
      break;
    case 1:	// own, renumerate
      {
	//cout << "renum\n";
	vector<Player>	vp;
	vector<unsigned>	vi;
	map<unsigned, unsigned>	trans;
	iterator		ip, ep=end();

	vp.reserve( n );
	vi.reserve( n );

	//cout << "build list\n";
	for( ip=begin(); ip!=ep; ++ip )
	  if( (*ip).second.network == 0 )	// local
	    {
	      vp.push_back( (*ip).second );
	      vi.push_back( (*ip).first );
	    }
	if( vp.size() != n )
	  cerr << "BUG: Renumerated players don't match actual number\n";
	// erase
	//cout << "erasing\n";
	for( i=0; i<vi.size(); ++i )
	  removePlayer( vi[i] );
	// re-build list
	//cout << "adding\n";
	unsigned	p = vp.size(), k;
	if( n < p )
	  p = n;
	for( i=0; i<p; ++i )
	  {
	    addPlayer( m.players[i] );
	    (*this)[ m.players[i] ] = vp[i];
	    trans[ vi[i] ] = m.players[i];
	  }
	//cout << "adding not numbered players\n";
	for( ; i<vp.size(); ++i )
	  {
	    k = newPlayer();
	    (*this)[ k ] = vp[i];
	  }
	//cout << "adding last\n";
	for( ; i<n; ++i )
	  addPlayer( m.players[i] );
	//cout << "OK\n";
	emit playersRenumed( trans );
      }
      break;
    case 3:	// removed players
      for( i=0; i<n; ++i )
	removePlayer( m.players[i] );
      break;
    default:
      break;
    }
  emit playersChanged();
}


void QPlayerServer::clientNumPlayersReceived( const NetMessage & msg )
{
  // this function should be useless... (and does nothing)
  const MessageNumPlayers	&m = (MessageNumPlayers &) msg;

  cout << "players already on network : " << m.num << endl;
}


void QPlayerServer::clientStartGame( const NetMessage & msg )
{
  const MessageStartGame	& m = (MessageStartGame &) msg;

  /*cout << "Start game : series : " << m.series << "\nlevel " << m.level 
    << "\nrandom seed : " << m.randomSeed << endl;*/

  game.allRollsMustExit = m.rollExitFlag;

  emit gameStarted( m.level, m.randomSeed, m.series );
}


void QPlayerServer::clientPauseGame( const NetMessage & msg )
{
  MessagePauseGame	& m = (MessagePauseGame &) msg;

  //cout << "Pause : " << (int) m.state << endl;
  emit gamePaused( m.state );
}


void QPlayerServer::clientStopGame( const NetMessage & )
{
  emit gameStopped();
}


void QPlayerServer::clientKeyEvent( const NetMessage & )
{
  cout << "Key event from server\n";
}


void QPlayerServer::clientEndTurn( const NetMessage & msg )
{
  MessageEndTurn	& m = (MessageEndTurn &) msg;
  map<unsigned, roll::Key>::const_iterator	ik, ek = m.keys.end();
  iterator		ip, ep = end();

  //cout << "End turn " << m.keys.size() << " keys\n";

  if( m.keys.size() != size() )
    {
      cerr << "number of players mismatch - bug ?\n";
      for( ip=begin(); ip!=ep; ++ip )
	(*ip).second.kbd = Game::K_NONE;
    }

  for( ik=m.keys.begin(); ik!=ek; ++ik )
    {
      iterator	ip = find( (*ik).first );
      if( ip == ep )
	cerr << "Inexistant player " << (*ik).first 
	     << "receives keys ! BUG...\n";
      else
	{
	  (*ip).second.kbd = (*ik).second.key;
	  (*ip).second.fire = (*ik).second.fire;
	}
    }

  // answer
  MessageOK	mok;
  _socket->writeMessage( mok );

  emit turnEnded();
}


//	commands from upper-level (game, main window...)

void QPlayerServer::startGame( unsigned level, unsigned rseed, 
			       const string & series )
{
  ServerSocket	*sk = dynamic_cast<ServerSocket *>( _socket );
  if( sk )
    {
      MessageStartGame	m;
      m.level = level;
      m.randomSeed = rseed;
      m.series = series;
      m.rollExitFlag = game.allRollsMustExit;

      sk->writeMessage( m );
    }
}


void QPlayerServer::stopGame()
{
  ServerSocket	*sk = dynamic_cast<ServerSocket *>( _socket );
  if( sk )
    {
      MessageStopGame	m;
      sk->writeMessage( m );
    }
}


void QPlayerServer::pauseGame( bool state )
{
  //cout << "QPlayerServer::pauseGame " << state << endl;
  MessagePauseGame	m;
  m.state = state;
  _socket->writeMessage( m );
}


void QPlayerServer::endTurn()
{
  //static unsigned count = 0;
  if( mode() == Server )
    {
      unsigned		n;
      n = d->clients.size();
      if( d->readyClients.size() == n )
	emitEndTurn();	// really ends turn, notify clients
      else
	{
	  cout << "Warning: turn ends with " << n - d->readyClients.size() 
	       << " / " << n << " clients not done\n";
	  cout << "we're waiting for him/them...\n";
	  d->waitingToComplete = true;
	}
    }
}


void QPlayerServer::emitEndTurn()
{
  d->readyClients.clear();
  d->waitingToComplete = false;

  //cout << "endTurn " << count++ << endl;
  MessageEndTurn	m;
  iterator		ip, ep = end();

  for( ip=begin(); ip!=ep; ++ip )
    {
      roll::Key	k;
      k.key = (*ip).second.kbd;
      k.fire = (*ip).second.fire;
      m.keys[ (*ip).first ] = k;
    }

  ServerSocket	*ss = (ServerSocket *) _socket;
  d->clients = ss->connections();
  _socket->writeMessage( m );
  emit turnEndEmitted();
}


void QPlayerServer::emitKey( unsigned player, bool press, unsigned key )
{
  MessageKey	m;
  m.player = player;
  m.key = key;
  m.pressRelease = press;
  _socket->writeMessage( m );
}


