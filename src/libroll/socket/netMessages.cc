/***************************************************************************
                          netMessages.cc  -  description
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

#include <roll/socket/netMessages.h>
#include <roll/socket/playerServerSocket.h>
#include <iostream>

using namespace roll;
using namespace std;


NetMessage* NetMessage::create( unsigned char code )
{
  switch( code )
    {
    case START_GAME:
      return( new MessageStartGame );
    case RECEIVE_OK:
      return( new MessageOK );
    case STOP_GAME:
      return( new MessageStopGame );
    case TEXT:
      return( new MessageText );
    case NUMPLAYERS:
      return( new MessageNumPlayers );
    case ASSIGNPLAYERS:
      return( new MessageAssignPlayers );
    case PAUSE_GAME:
      return( new MessagePauseGame );
    case KEYEVENT:
      return( new MessageKey );
    case END_TURN:
      return( new MessageEndTurn );
    default:
      cout << "NetMessage::create : unrecognized message code " 
	   << code << endl;
      break;
    }

  cerr << "Unrecognized message code - can't build\n";
  return( 0 );
}


NetMessage::NetMessage( unsigned char c ) : _code ( c )
{
}


NetMessage::~NetMessage()
{
}


bool NetMessage::canRead( unsigned len ) const
{
  return( len == size() );
}


bool NetMessage::read( const char* buffer, unsigned len )
{
  if( canRead( len ) )
    {
      memcpy( ( (char *) this ) + sizeof( _code ), buffer, len );
      return( true );
    }
  return( false );
}


char* NetMessage::write() const
{
  unsigned	len = size();
  char		*buf = new char[ len ];

  memcpy( buf, this, len );
  return( buf );
}


// ------------------

MessageOK::MessageOK() : NetMessage( RECEIVE_OK )
{
}


MessageOK::~MessageOK()
{
}


unsigned MessageOK::size() const
{
  return( 0 );
}


// -------------------

MessageText::MessageText() : NetMessage( TEXT )
{
}


MessageText::~MessageText()
{
}


unsigned MessageText::size() const
{
  return( text.size() );
}


void MessageText::setText( const string & txt )
{
  text = txt;
}


bool MessageText::canRead( unsigned ) const
{
  return( true );
}


bool MessageText::read( const char* buffer, unsigned len )
{
  text = buffer;
  unsigned	n = 0;

  //cout << "TEXT MESSAGE :\n";

  while( n < len )
    {
      //cout << text << endl;
      n += text.size();
      if( n < len )
	cout << "message longer than text : " << n 
	     << " instead of " << len << endl;
    }

  return( true );
}


char* MessageText::write() const
{
  unsigned	len = size();
  char		*buf = new char[ len ];

  memcpy( buf, text.c_str(), len );
  return( buf );
}


// ---------------------


MessageAssignPlayers::MessageAssignPlayers() 
  : NetMessage( ASSIGNPLAYERS ), whose( 0 )
{
}


MessageAssignPlayers::~MessageAssignPlayers()
{
}


unsigned MessageAssignPlayers::size() const
{
  return( players.size() * sizeof( unsigned long ) + sizeof( unsigned long ) );
}


bool MessageAssignPlayers::canRead( unsigned len ) const
{
  return( len >= sizeof( unsigned long ) 
	  && len % sizeof( unsigned long ) == 0 );
}


bool MessageAssignPlayers::read( const char* buffer, unsigned len )
{
  players.clear();
  len = len / sizeof( unsigned long ) - 1;

  unsigned	i;
  unsigned long	*b = (unsigned long *) buffer;

  whose = native( *b++ );

  for( i=0; i<len; ++i )
    players.push_back( native( b[i] ) );
  
  return( true );
}


char* MessageAssignPlayers::write() const
{
  char		*buf = new char[ size() ];
  unsigned long	*b = (unsigned long *) buf;
  unsigned	i, n = players.size();

  * b++ = little_endian( (unsigned long) whose );

  for( i=0; i<n; ++i )
    b[i] = little_endian( (unsigned long) players[i] );
  return( buf );
}


// -----------------------


MessageNumPlayers::MessageNumPlayers() 
  : NetMessage( NUMPLAYERS ), num( 0 )
{
}


MessageNumPlayers::~MessageNumPlayers()
{
}


unsigned MessageNumPlayers::size() const
{
  return( sizeof( unsigned long ) );
}


bool MessageNumPlayers::read( const char* buffer, unsigned )
{
  num = native( * (unsigned long *) buffer );
  return( true );
}


char* MessageNumPlayers::write() const
{
  char	*buf = new char[ size() ];
  *(unsigned long *) buf = little_endian( (unsigned long) num );
  return( buf );
}


// ------------------------


MessageStartGame::MessageStartGame() 
  : NetMessage( START_GAME ), level( 0 ), randomSeed( 0 ), rollExitFlag( 0 )
{
}


MessageStartGame::~MessageStartGame()
{
}


unsigned MessageStartGame::size() const
{
  return( sizeof( unsigned long ) * 3 + 1 + series.length() );
}


bool MessageStartGame::canRead( unsigned len ) const
{
  return( len >= sizeof( unsigned long ) * 3 + 1 );
}


bool MessageStartGame::read( const char* buffer, unsigned l )
{
  const unsigned long	*bu = (const unsigned long *) buffer;

  level = native( *bu++ );
  randomSeed = native( *bu++ );

  unsigned long	len = native( *bu++ );
  const char	*bc = (const char *) bu;
  bool		res = true;

  rollExitFlag = (unsigned char) *bc++;

  //cout << "msg len ; " << len << endl;
  l -= sizeof( unsigned long) * 3 + 1;
  if( len > l )
    {
      cerr << "MessageStartGame too short ! Truncating\n";
      len = l;
      res = false;
    }
  char	*c = new char[ len + 1 ];
  memcpy( c, bc, len );
  c[len] = 0;
  series = c;
  delete[] c;

  return( true );
}


char* MessageStartGame::write() const
{
  char		*buf = new char[ size() ];
  unsigned long	*bu = (unsigned long *) buf;

  *bu++ = little_endian( (unsigned long) level );
  *bu++ = little_endian( (unsigned long) randomSeed );
  *bu++ = little_endian( (unsigned long) series.length() );

  char	*bc = (char *) bu;
  *bc++ = (char) rollExitFlag;
  memcpy( bc, series.c_str(), series.length() );

  return( buf );
}


// --------------------------


MessageStopGame::MessageStopGame() : NetMessage( STOP_GAME )
{
}


MessageStopGame::~MessageStopGame()
{
}


unsigned MessageStopGame::size() const
{
  return( 0 );
}


// ---------------------------


MessagePauseGame::MessagePauseGame() : NetMessage( PAUSE_GAME ), state( 0 )
{
}


MessagePauseGame::~MessagePauseGame()
{
}


unsigned MessagePauseGame::size() const
{
  return( 1 );
}


bool MessagePauseGame::read( const char* buffer, unsigned )
{
  state = buffer[0];
  return( true );
}


char* MessagePauseGame::write() const
{
  char	*buf = new char[ 1 ];

  buf[0] = state;
  return( buf );
}


// -----------------------------


MessageEndTurn::MessageEndTurn() : NetMessage( END_TURN )
{
}


MessageEndTurn::~MessageEndTurn()
{
}


unsigned MessageEndTurn::size() const
{
  return( keys.size() * ( sizeof( unsigned long ) 
			  + sizeof( unsigned short ) * 2 ) );
}


bool MessageEndTurn::canRead( unsigned len ) const
{
  return( len % ( sizeof( unsigned long ) + sizeof( Key ) ) == 0 );
}


bool MessageEndTurn::read( const char* buffer, unsigned len )
{
  unsigned long		i, p;
  const unsigned long	*bu = (const unsigned long *) buffer;
  const unsigned short	*bs;

  len = len / ( sizeof( unsigned long ) + sizeof( Key ) );

  for( i=0; i<len; ++i )
    {
      p = native( *bu++ );
      Key	k;
      bs = (const unsigned short *) bu;
      k.key = native( *bs++ );
      k.fire = native( *bs++ );
      keys[ p ] = k;
      bu = (const unsigned long *) bs;
    }
  if( keys.size() != len )
    cout << "MessageEndTurn decoding failed !\n";

  return( true );
}


char* MessageEndTurn::write() const
{
  char					*buf = new char[ size() ];
  unsigned long				*bu = (unsigned long *) buf;
  unsigned short			*bs;
  map<unsigned, Key>::const_iterator	ik, ek = keys.end();

  for( ik=keys.begin(); ik!=ek; ++ik )
    {
      *bu++ = little_endian( (unsigned long) (*ik).first );
      bs = (unsigned short *) bu;
      *bs++ = little_endian( (*ik).second.key );
      *bs++ = little_endian( (*ik).second.fire );
      bu = (unsigned long *) bs;
    }

  return( buf );
}


// ------------------------------


MessageKey::MessageKey() 
  : NetMessage( KEYEVENT ), player( 0 ),  key( 0 ), pressRelease( 0 )
{
}


MessageKey::~MessageKey()
{
}


unsigned MessageKey::size() const
{
  return( sizeof( unsigned long ) * 2 + sizeof( unsigned char ) );
}


bool MessageKey::read( const char* buffer, unsigned )
{
  const unsigned long	*bu = (const unsigned long *) buffer;
  player = native( *bu++ );
  key = native( *bu++ );
  pressRelease = *(const unsigned char *) bu;
  return( true );
}


char* MessageKey::write() const
{
  char		*buf = new char[ size() ];
  unsigned	*bu = (unsigned *) buf;

  *bu++ = little_endian( (unsigned long) player );
  *bu++ = little_endian( (unsigned long) key );
  *(unsigned char *) bu = pressRelease;
  return( buf );
}


