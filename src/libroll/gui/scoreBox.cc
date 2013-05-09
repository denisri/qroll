/***************************************************************************
                          scoreBox.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis RiviÃ¯Â¿Â½e
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


#include <roll/gui/scoreBox.h>
#include <qtimer.h>
#include <iostream>
#include <roll/game/vars.h>
#include <roll/game/playerServer.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;


QRScoreBox::QRScoreBox( QWidget *parent, const char *name ) 
  : QWidget( parent )
{
  if( name )
    setObjectName( name );
  setMinimumWidth( 128 );
  QPalette palette;
  palette.setColor( backgroundRole(), QColor( 230, 230, 335 ) );
  setPalette( palette );

#define PanelStyle	QFrame::Box | QFrame::Raised
  _timeLabel = new QLabel( this );
  _timeLabel->setObjectName( "timeLabel" );
  _timeLabel->setFrameStyle( PanelStyle );
  _time = new QLCDNumber( this );
  _time->setObjectName( "time" );
  _timeLabel->resize( 100, 20 );
  _time->resize( 60, 20 );

  _scoreLabel = new QLabel( this );
  _scoreLabel->setObjectName( "scoreLabel" );
  _scoreLabel->setFrameStyle( PanelStyle );
  _score = new QLCDNumber( this );
  _score->setObjectName( "score" );
  _scoreLabel->resize( 100, 20 );
  _score->resize( 60, 20 );

  _livesLabel = new QLabel( this );
  _livesLabel->setObjectName( "livesLabel" );
  _livesLabel->setFrameStyle( PanelStyle );
  _lives = new QLCDNumber( this );
  _lives->setObjectName( "lives" );
  _livesLabel->resize( 100, 20 );
  _lives->resize( 60, 20 );

  _diamsLabel = new QLabel( this );
  _diamsLabel->setObjectName( "diamsLabel" );
  _diamsLabel->setFrameStyle( PanelStyle );
  _diams = new QLCDNumber( this );
  _diams->setObjectName( "diams" );
  _diamsLabel->resize( 100, 20 );
  _diams->resize( 60, 20 );

  int	fontsize = 12;

  _timeLabel->setAlignment( Qt::AlignCenter );
  _timeLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  _timeLabel->setText( tr( "TIME :" ) );
  _timeLabel->move( 15, 10 );
  _timeLabel->setEnabled( false );

  _time->move( 35, 35 );

  _scoreLabel->setAlignment( Qt::AlignCenter );
  _scoreLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  _scoreLabel->setText( tr( "SCORE :" ) );
  _scoreLabel->move( 15, 65 );
  _scoreLabel->setEnabled( false );

  _score->move( 35, 90 );

  _livesLabel->setAlignment( Qt::AlignCenter );
  _livesLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  _livesLabel->setText( tr( "LIVES :" ) );
  _livesLabel->move( 15, 120 );
  _livesLabel->setEnabled( false );

  _lives->move( 35, 145 );

  _diamsLabel->setAlignment( Qt::AlignCenter );
  _diamsLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  _diamsLabel->setText( tr( "DIAMS :" ) );
  _diamsLabel->move( 15, 175 );
  _diamsLabel->setEnabled( false );

  _diams->move( 35, 200 );

  QLabel	*keyLabel = new QLabel( this );
  keyLabel->setObjectName( "keyLabel" );
  keyLabel->setFrameStyle( PanelStyle );
  keyLabel->setGeometry( 15, 230, 60, 20 );
  keyLabel->setAlignment( Qt::AlignCenter );
  keyLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  keyLabel->setText( tr( "KEY :" ) );
  keyLabel->setEnabled( false );

  _key = new QFrame( this );
  _key->setObjectName( "key" );
  _key->setFrameStyle( PanelStyle );
  _key->setGeometry( 85, 225, 32, 32 );

  QLabel	*bombsLabel = new QLabel( this );
  bombsLabel->setObjectName( "bombsLabel" );
  bombsLabel->setFrameStyle( PanelStyle );
  bombsLabel->setGeometry( 15, 265, 100, 20 );
  bombsLabel->setAlignment( Qt::AlignCenter );
  bombsLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  bombsLabel->setText( tr( "BOMBS :" ) );
  bombsLabel->setEnabled( false );

  for( int i=0; i<4; ++i )
    {
      _bombsT[i*2] = new QFrame( this );
      _bombsT[i*2]->setFrameStyle( PanelStyle );
      _bombsT[i*2]->setGeometry( 12, 293+25*i, 16, 16 );
      _bombs[i*2] = new QLCDNumber( this );
      _bombs[i*2]->setDigitCount( 2 );
      _bombs[i*2]->setGeometry( 32, 290+25*i, 30, 20 );

      _bombsT[i*2+1] = new QFrame( this );
      _bombsT[i*2+1]->setFrameStyle( PanelStyle );
      _bombsT[i*2+1]->setGeometry( 68, 293+25*i, 16, 16 );
      _bombs[i*2+1] = new QLCDNumber( this );
      _bombs[i*2+1]->setDigitCount( 2 );
      _bombs[i*2+1]->setGeometry( 88, 290+25*i, 30, 20 );
    }

  resize( 128, 384 );

  /*_timer = new QTimer( this, "timer" );
    connect(_timer, SIGNAL(timeout()), this, SLOT(oneLessSecond()));*/
}


QRScoreBox::~QRScoreBox()
{
}


void QRScoreBox::setTime( unsigned t )
{
  _time->display( (int) t );
}


void QRScoreBox::setScore( unsigned score )
{
  _score->display( (int) score );
}


void QRScoreBox::oneLessSecond()
{
  _time->display( game.tbct.time() );
}


void QRScoreBox::changeStage()
{
  changeScore();
  oneLessSecond();
}


void QRScoreBox::setLives( unsigned lives )
{
  _lives->display( (int) lives );
}


void QRScoreBox::setDiams( unsigned diams )
{
  _diams->display( (int) diams );
}


void QRScoreBox::changeScore()
{
  Player	&pl = (*game.tbct.players)[ _player ];

  if( diams() != game.tbct.diams() )
    setDiams( game.tbct.diams() );

  if( lives() != pl.lives )
    setLives( pl.lives );

  if( score() != pl.score )
    setScore( pl.score );
}


void QRScoreBox::setPlayer( unsigned num )
{
  _player = num;
  changeScore();
}






