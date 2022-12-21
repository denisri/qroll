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
#include <roll/game/key.h>
#include <roll/sound/soundProcessor.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>

using namespace roll;
using namespace std;


QRScoreBox::QRScoreBox( const QPixmap * const * sprites, QWidget *parent, 
                        const char *name ) 
  : QWidget( parent ), _sprites( sprites ), _bombToLaunch( -1 ), _keyid( -1 )
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
  QVBoxLayout *lay = new QVBoxLayout( _key );
  _key->setLayout( lay );
  lay->setContentsMargins( 0, 0, 0, 0 );

  QLabel	*bombsLabel = new QLabel( this );
  bombsLabel->setObjectName( "bombsLabel" );
  bombsLabel->setFrameStyle( PanelStyle );
  bombsLabel->setGeometry( 15, 265, 100, 20 );
  bombsLabel->setAlignment( Qt::AlignCenter );
  bombsLabel->setFont( QFont( "Helvetica", fontsize, QFont::Bold ) );
  bombsLabel->setText( tr( "BOMBS :" ) );
  bombsLabel->setEnabled( false );

  QButtonGroup *bgrp = new QButtonGroup( this );
  bgrp->setExclusive( true );
  _bombgroup = bgrp;

  for( int i=0; i<4; ++i )
    {
      QPushButton *bt = new QPushButton( this );
      _bombsT[i*2] = bt;
      bt->setCheckable( true );
      bgrp->addButton( bt, i*2 );
      bt->setFocusPolicy( Qt::NoFocus );
      bt->setGeometry( 11, 290+25*i, 20, 20 );
      _bombs[i*2] = new QLCDNumber( this );
      _bombs[i*2]->setDigitCount( 2 );
      _bombs[i*2]->setGeometry( 33, 290+25*i, 30, 20 );

      bt = new QPushButton( this );
      _bombsT[i*2+1] = bt;
      bt->setCheckable( true );
      bgrp->addButton( bt, i*2+1 );
      bt->setFocusPolicy( Qt::NoFocus );
      bt->setGeometry( 67, 290+25*i, 20, 20 );
      _bombs[i*2+1] = new QLCDNumber( this );
      _bombs[i*2+1]->setDigitCount( 2 );
      _bombs[i*2+1]->setGeometry( 89, 290+25*i, 30, 20 );
    }

#if QT_VERSION >= 0x050f00
  connect( bgrp, SIGNAL( idClicked( int ) ),
           this, SLOT( bombSelected( int ) ) );
#else
  connect( bgrp, SIGNAL( buttonClicked( int ) ),
           this, SLOT( bombSelected( int ) ) );
#endif

  resize( 128, 384 );
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

  map<unsigned, unsigned>::iterator ib, eb = pl.bombs.end();
  unsigned i = 0;

  for( ib=pl.bombs.begin(); ib!=eb; ++ib )
  {
    if( (int) ib->second != _bombs[ ib->first ]->intValue() )
      setBombs( ib->first, ib->second );
  }
  for( i=0; i<8; ++i )
  {
    if( pl.bombs.find( i ) == eb && _bombs[ i ]->intValue() != 0 )
      setBombs( i, 0 );
  }
  if( (int) pl.bombToLaunch != _bombToLaunch )
  {
    if( _bombToLaunch >= 0 )
      _bombsT[ _bombToLaunch ]->setPalette( QPalette() );
    _bombToLaunch = pl.bombToLaunch;
    _bombsT[ _bombToLaunch ]->setPalette( QColor( 100, 100, 255 ) );
    _bombsT[ _bombToLaunch ]->setChecked( true );
  }

  if( _keyid != pl.key )
    setKey( pl.key );
}


void QRScoreBox::setPlayer( unsigned num )
{
  _player = num;
  changeScore();
}


void QRScoreBox::setBombs( unsigned bombid, unsigned num )
{
  _bombs[ bombid ]->display( (int) num );
  if( num == 0 )
  {
    _bombsT[ bombid ]->setIcon( QIcon() );
  }
  else // if( !label )
  {
    static unsigned bombspr[8] = { 33, 0x121, 0x199, 0x124, 0, 0, 0, 0 };
    QPixmap pix = _sprites[ bombspr[bombid] ]->scaled( 16, 16 );
    _bombsT[ bombid ]->setIcon( pix );
  }
}


void QRScoreBox::setKey( int key )
{
  _keyid = key;
  QLabel *label = _key->findChild<QLabel *>();
  if( label )
    delete label;
  if( key >= 0 )
  {
    label = new QLabel( _key );
    label->setPixmap( *_sprites[ RKey::keyOfNum( key ) ] );
    _key->layout()->addWidget( label );
  }
}


void QRScoreBox::bombSelected( int num )
{
  out << "bombSelected " << num << endl;
  Player        &pl = (*game.tbct.players)[ _player ];
  if( _bombgroup->checkedId() >= 0 )
    pl.bombToLaunch = _bombgroup->checkedId();
}



