/***************************************************************************
                          playField.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis RiviÃ¯Â¿Â½e
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


#include <roll/gui/playField.h>
#include <roll/gui/mainWin.h>
#include <roll/gui/qGameField.h>
#ifndef ANDROID
#include <roll/gui/qglGameField.h>
#endif
#include <roll/gui/scoreBox.h>
#include <roll/game/vars.h>
#include <roll/gui/editPalette.h>
#include <iostream>
#if QT_VERSION >= 0x040000
#include <qnamespace.h>
#include <qlayout.h>
#else
#include <qkeycode.h>
#include <qvbox.h>
#endif
#include <qevent.h>
#include <qlayout.h>
#include <qcursor.h>
// *** DEBUG ***
//#include <qapplication.h>
//#include <qfocusdata.h>

using namespace roll;
using namespace std;
#if QT_VERSION >= 0x040000
using namespace Qt;
#endif


namespace roll
{

  struct QRPlayField_Private
  {
    QRPlayField_Private() 
      : parentMW( 0 ), scoreBox( 0 ), opengl( false ), gameField( 0 ), 
	gameWid( 0 ), game( 0 ), dragging( false ), interndrag( false ) {}

    const QRMainWin	*parentMW;
    QRScoreBox		*scoreBox;
    bool		opengl;
    RGameField		*gameField;
    QWidget		*gameWid;
    QWidget		*game;
    bool		dragging;
    QPoint		dragorg;
    int			draglvlorgx;
    int			draglvlorgy;
    bool                interndrag;
  };

}


map<int, QRPlayField::KeyCode>	QRPlayField::KeyMapMono;
map<int, QRPlayField::KeyCode>	QRPlayField::KeyMapMulti;


QRPlayField::QRPlayField( const QRMainWin* parentMW, bool usegl, 
			  QWidget *parent, const char *name ) 
  : QWidget( parent ), d( new QRPlayField_Private )
{
  d->parentMW = parentMW;

  initKeyCodes();

  QHBoxLayout	*lay= new QHBoxLayout( this );

  d->game = new QWidget( this );
  QVBoxLayout *vg = new QVBoxLayout( d->game );
  d->game->setLayout( vg );
  d->opengl = !usegl;
  setUseOpenGL( usegl );

  d->scoreBox = new QRScoreBox( this, "RRScoreBox" );
  d->scoreBox->setFixedWidth( d->scoreBox->width() );

  lay->addWidget( d->game );
  lay->addWidget( d->scoreBox );
  resize( d->gameWid->width() + d->scoreBox->width(), d->gameWid->height() );

  connect( d->parentMW, SIGNAL( scoreChanged() ), 
	   d->scoreBox, SLOT( changeScore() ) );
  connect( d->parentMW, SIGNAL( stageChanged( unsigned ) ), this, 
	   SLOT( stageChanged( unsigned ) ) );
  connect( this, SIGNAL( keyPressed( const QRPlayField::KeyCode &, 
				     const QRPlayField * ) ), 
	   d->parentMW, 
	   SLOT( receiveKeyPressed( const QRPlayField::KeyCode &, 
				    const QRPlayField * ) ) );
  connect( this, 
	   SIGNAL( keyReleased( const QRPlayField::KeyCode &, 
				const QRPlayField * ) ), 
	   d->parentMW, 
	   SLOT( receiveKeyReleased( const QRPlayField::KeyCode &, 
				     const QRPlayField * ) ) );
  connect( this, SIGNAL( playFieldClosed( QRPlayField * ) ), d->parentMW, 
	   SLOT( closePlayField( QRPlayField * ) ) );
  connect( d->parentMW, SIGNAL( removePlayer( unsigned ) ), this, 
	   SLOT( removePlayer( unsigned ) ) );
  connect( d->parentMW, 
	   SIGNAL( renumPlayers( const roll::RenumList & ) ), this, 
	   SLOT( renumPlayers( const roll::RenumList & ) ) );

  connect( d->parentMW, SIGNAL( timeTick() ), d->scoreBox, 
	   SLOT( oneLessSecond() ) );
  connect( this, SIGNAL( paintStart( int, int, bool, QRPlayField* ) ), 
	   d->parentMW, SLOT( paintStart( int, int, bool, QRPlayField* ) ) );
  connect( this, SIGNAL( paintMove( int, int, bool, QRPlayField* ) ), 
	   d->parentMW, SLOT( paintMove( int, int, bool, QRPlayField* ) ) );
  connect( this, SIGNAL( paintStop( int, int, bool, QRPlayField* ) ), 
	   d->parentMW, SLOT( paintStop( int, int, bool, QRPlayField* ) ) );

  setFocusPolicy( StrongFocus );
  setFocus();
  //setActiveWindow();
  /*cout << "toplevel     : " << isTopLevel() << endl;
  cout << "activeWindow : " << isActiveWindow() << endl;
  cout << "hasFocus     : " << hasFocus() << endl;
  cout << "focusEnabled : " << isFocusEnabled() << endl;
  cout << "focusPolicy  : " << (int) focusPolicy() << "   (StrongFocus: " 
       << (int) StrongFocus << ", NoFocus : " << (int) NoFocus << ")\n";
  cout << "this         : " << this << endl;
  cout << "focusWidget  : " << qApp->focusWidget() << endl;
  cout << "parentMW     : " << parentMW << endl;
  cout << "focusData    : " << focusData()->count() << " wid, act : " 
  << focusData()->focusWidget() << endl;*/
}


QRPlayField::~QRPlayField()
{
}


void QRPlayField::initKeyCodes()
{
  if( KeyMapMono.size() > 0 )
    return;	// dÃ¯Â¿Â½Ã¯Â¿Â½fait...

  //	joueur 0 mono-joueur
  KeyMapMono[Key_Up] = KeyCode( 0, game.K_UP );
  KeyMapMono[Key_Down] = KeyCode( 0, game.K_DOWN );
  KeyMapMono[Key_Left] = KeyCode( 0, game.K_LEFT );
  KeyMapMono[Key_Right] = KeyCode( 0, game.K_RIGHT );
  KeyMapMono[Key_Space] = KeyCode( 0, game.K_BOMB );
  KeyMapMono[Key_Escape] = KeyCode( 0, game.K_ABORT );
  KeyMapMono[Key_Tab] = KeyCode( 0, game.K_CHANGEBOMB );
  KeyMapMono[Key_Control] = KeyCode( 0, game.K_FIRE );

  //	joueur 0 multi-joueurs
  KeyMapMulti[Key_Up] = KeyCode( 0, game.K_UP );
  KeyMapMulti[Key_Down] = KeyCode( 0, game.K_DOWN );
  KeyMapMulti[Key_Left] = KeyCode( 0, game.K_LEFT );
  KeyMapMulti[Key_Right] = KeyCode( 0, game.K_RIGHT );
  KeyMapMulti[Key_Return] = KeyCode( 0, game.K_BOMB );
  KeyMapMulti[Key_F12] = KeyCode( 0, game.K_ABORT );
  KeyMapMulti[Key_Shift] = KeyCode( 0, game.K_CHANGEBOMB );
  KeyMapMulti[Key_Control] = KeyCode( 0, game.K_FIRE );

  //	joueur 1 multi-joueurs (clavier azerty...)
  KeyMapMulti[Key_Z] = KeyCode( 1, game.K_UP );
  KeyMapMulti[Key_W] = KeyCode( 1, game.K_DOWN );
  KeyMapMulti[Key_Q] = KeyCode( 1, game.K_LEFT );
  KeyMapMulti[Key_D] = KeyCode( 1, game.K_RIGHT );
  KeyMapMulti[Key_Space] = KeyCode( 1, game.K_BOMB );
  KeyMapMulti[Key_Escape] = KeyCode( 1, game.K_ABORT );
  KeyMapMulti[Key_Tab] = KeyCode( 1, game.K_CHANGEBOMB );
  KeyMapMulti[Key_Alt] = KeyCode( 1, game.K_FIRE );

  //	joueur 2 multi-joueurs
  KeyMapMulti[Key_O] = KeyCode( 2, game.K_UP );
  KeyMapMulti[Key_Semicolon] = KeyCode( 2, game.K_DOWN );
  KeyMapMulti[Key_K] = KeyCode( 2, game.K_LEFT );
  KeyMapMulti[Key_M] = KeyCode( 2, game.K_RIGHT );
  KeyMapMulti[Key_B] = KeyCode( 2, game.K_BOMB );
  KeyMapMulti[Key_F5] = KeyCode( 2, game.K_ABORT );
  KeyMapMulti[Key_T] = KeyCode( 2, game.K_CHANGEBOMB );
  KeyMapMulti[Key_H] = KeyCode( 2, game.K_FIRE );
}


unsigned QRPlayField::player() const
{
  return( d->gameField->player() );
}


void QRPlayField::resizeEvent( QResizeEvent* )
{
  //out << "QRPlayField::resizeEvent\n";
  d->gameWid->setGeometry( 0, 0, width() - d->scoreBox->width(), height() );
  d->scoreBox->setGeometry( width() - d->scoreBox->width(), 0, 
			    d->scoreBox->width(), height() );
}


void QRPlayField::stageChanged( unsigned n )
{
  if( n != (unsigned) game.tb )
    cerr << "QRPlayField::stageChanged : level not updated in game yet !\n";
  d->scoreBox->changeStage();
  d->gameField->reset();
}


void QRPlayField::keyPressedEvent( QKeyEvent* key )
{
  bool	ctrl, shft, alt;

  //cout << "QRPlayField::keyPressedEvent\n";
  if( game.running )
    {
      if( key->isAutoRepeat() )
	{
	  key->ignore();
	  return;
	}

      if( key->state() & ControlButton )
	ctrl = true;
      else ctrl = false;
      if( key->state() & ShiftButton )
	shft = true;
      else shft = false;
      if( key->state() & AltButton )
	alt = true;
      else alt = false;

      /*      _kbd[ Key_Control ] = ctl;
      if( _kbd[ k ] )
	{
	  //cout << "key ignored\n";
	  key->ignore();
	  return;
	}
	_kbd[ k ] = true; */

      map<int, KeyCode>::const_iterator	ik, fk;

      if( _players.size() <= 1 )
	{
	  ik = KeyMapMono.find( key->key() );
	  fk = KeyMapMono.end();
	}
      else
	{
	  ik = KeyMapMulti.find( key->key() );
	  fk = KeyMapMono.end();
	}

      if( ik != fk )	// keycode found
        {
	  /*cout << "key pressed : player " << (*ik).second.player
	    << ", keycode: " << (*ik).second.key << "\n";*/
	  KeyCode	k  = (*ik).second;

	  if( k.player < _players.size() )
	    {	// translate player number
	      unsigned	i;
	      set<unsigned>::iterator	ip;
	      for( i=0, ip=_players.begin(); i<k.player; ++i, ++ip );
	      k.player = *ip;
	      emit keyPressed( k, this );
	    }
	}
      else
	key->ignore();
      return;
    }

  //	Not running

  switch( key->key() )
    {
    case Key_Up:
      d->gameField->up();
      //emit displayFull();
      break;
    case Key_Down:
      d->gameField->down();
      //emit displayFull();
      break;
    case Key_Left:
      d->gameField->left();
      //emit displayFull();
      break;
    case Key_Right:
      d->gameField->right();
      //emit displayFull();
      break;
    default:
      key->ignore();
      break;
    }
}


void QRPlayField::keyReleasedEvent( QKeyEvent* key )
{
  bool	ctrl, shft, alt;

  //cout << "QRPlayField::keyPressedEvent\n";
  if( game.running )
    {
      if( key->isAutoRepeat() )
	{
	  key->ignore();
	  return;
	}

      if( key->state() & ControlButton )
	ctrl = true;
      else ctrl = false;
      if( key->state() & ShiftButton )
	shft = true;
      else shft = false;
      if( key->state() & AltButton )
	alt = true;
      else alt = false;

      /*      _kbd[ Key_Control ] = ctl;
      if( _kbd[ k ] )
	{
	  //cout << "key ignored\n";
	  key->ignore();
	  return;
	}
	_kbd[ k ] = true; */

      map<int, KeyCode>::const_iterator	ik, fk;

      if( _players.size() <= 1 )
	{
	  ik = KeyMapMono.find( key->key() );
	  fk = KeyMapMono.end();
	}
      else
	{
	  ik = KeyMapMulti.find( key->key() );
	  fk = KeyMapMono.end();
	}

      if( ik != fk )	// keycode found
        {
	  KeyCode	k  = (*ik).second;

	  if( k.player < _players.size() )
	    {	// translate player number
	      unsigned	i;
	      set<unsigned>::iterator	ip;
	      for( i=0, ip=_players.begin(); i<k.player; ++i, ++ip );
	      k.player = *ip;
	      emit keyReleased( k, this );
	    }
	}
      else
	key->ignore();
      return;
    }
  else
    key->ignore();
}


void QRPlayField::setPlayers( const set<unsigned> & pl )
{
  _players = pl;
  if( _players.find( d->gameField->player() ) == _players.end() )
    {
      if( _players.size() > 0 )
	{
	  d->gameField->setPlayer( *_players.begin() );
	  d->scoreBox->setPlayer( *_players.begin() );
	}
      else
	{
	  d->gameField->setPlayer( 0 );
	  d->scoreBox->setPlayer( 0 );
	}
    }
}


void QRPlayField::setPlayer( unsigned p )
{
  _players.insert( p );
  d->gameField->setPlayer( p );
  d->scoreBox->setPlayer( p );
}


void QRPlayField::closeEvent( QCloseEvent* e )
{
  emit playFieldClosed( this );
  e->accept();
}


void QRPlayField::removePlayer( unsigned num )
{
  if( player() == num )
    close();
  if( _players.find( num ) != _players.end() )
    _players.erase( num );
}


void QRPlayField::renumPlayers( const RenumList & nums )
{
  //cout << "QRPlayField::renumPlayers - " << nums.size() << " renums\n";

  map<unsigned, unsigned>::const_iterator	i, f = nums.end();
  set<unsigned>::iterator			j, fp = _players.end();
  set<unsigned>					newp;

  for( i=nums.begin(); i!=f; ++i )
    {
      j = _players.find( (*i).first );
      if( j != fp )
	{
	  newp.insert( (*i).second );
	  /*cout << "renum player " << (*i).first << " -> " << (*i).second 
	    << endl;*/
	  if( player() == *j )
	    {
	      d->gameField->setPlayer( (*i).second );
	      d->scoreBox->setPlayer( (*i).second );
	      //cout << (*i).second << " is my player\n";
	    }
	}
    }
  _players = newp;
}


void QRPlayField::enableScaling( bool f )
{
  d->gameField->enableScaling( f );
}


bool QRPlayField::scalingEnabled() const
{
  return( d->gameField->scalingEnabled() );
}


float QRPlayField::scale() const
{
  return( d->gameField->scaleFactor() );
}


void QRPlayField::setGameSize( unsigned w, unsigned h )
{
  d->gameWid->resize( w, h );
  resize( d->gameWid->width() + d->scoreBox->width(), d->gameWid->height() );
}


QSize QRPlayField::sizeHint() const
{
  return( QSize( d->gameWid->width() + d->scoreBox->width(), 
		 d->gameWid->height() ) );
}


bool QRPlayField::usesOpenGL() const
{
  return( d->opengl );
}


void QRPlayField::setUseOpenGL( bool x )
{
  if( d->opengl != x )
    {
      d->opengl = x;
      unsigned	p;
      bool	old = false, enablesc = false;
      //float	sc = 1;

      if( d->gameField )
	{
	  p = player();
	  //sc = scale();
	  enablesc = scalingEnabled();
	  delete d->gameField;
	  old = true;
	}
      else
	p = 0;

#ifndef RR_NO_OPENGL
      if( x )
	{
	  QRGLGameField	*gf 
	    = new QRGLGameField( d->parentMW->sprites(), d->game, 
				 "GLGameField" );
	  d->gameField = gf;
	  d->gameWid = gf;
          d->game->layout()->addWidget( gf );
	}
      else
	{
#endif
	  QRGameField	*gf 
	    = new QRGameField( d->parentMW->sprites(), d->game, 
			       "QRGameField" );
	  d->gameField = gf;
	  d->gameWid = gf;
          d->game->layout()->addWidget( gf );
#ifndef RR_NO_OPENGL
	}
#endif
      d->gameField->setPlayer( p );

      connect( d->parentMW, SIGNAL( displayFull() ), 
	       d->gameWid, SLOT( displayFullSlot() ) );
      connect( d->parentMW, SIGNAL( displayHalf() ), 
	       d->gameWid, SLOT( displayHalfSlot() ) );

      connect( d->gameWid, SIGNAL( mousePress( QMouseEvent * ) ), this, 
	       SLOT( mousePress( QMouseEvent * ) ) );
      connect( d->gameWid, SIGNAL( mouseRelease( QMouseEvent * ) ), this, 
	       SLOT( mouseRelease( QMouseEvent * ) ) );
      connect( d->gameWid, SIGNAL( mouseMove( QMouseEvent * ) ), this, 
	       SLOT( mouseMove( QMouseEvent * ) ) );
      connect( d->gameWid, SIGNAL( displayFullDone( QWidget * ) ), this, 
	       SIGNAL( displayFullDone( QWidget * ) ) );

      if( old )
	{
	  d->gameWid->show();
	  adjustSize();
	}
      d->gameField->reset();
      //d->gameField->setSacle( sc );
      enableScaling( enablesc );
    }
}


void QRPlayField::mousePress( QMouseEvent* e )
{
  if( game.running )	// deactivate mouse if game is running
    return;

  d->dragging = true;

  switch( e->button() )
    {
    case LeftButton:
      posFromMouse( e->pos(), d->draglvlorgx, d->draglvlorgy );
      d->interndrag = false;
      if( theQRWin->editMode() )
	paint( e->pos() );
      break;
    case RightButton:	// drag image
      d->interndrag = true;
      d->dragorg = e->pos();
      d->draglvlorgx = (int) d->gameField->posX();
      d->draglvlorgy = (int) d->gameField->posY();
      setCursor( QCursor( SizeAllCursor ) );
      break;
    default:
      break;
    }
}


void QRPlayField::mouseRelease( QMouseEvent* e )
{
  d->dragging = false;
  if( d->interndrag )
    unsetCursor();
  else
    {
      int x, y;
      bool ok = posFromMouse( e->pos(), x, y );
      if( e->button() != LeftButton )
	ok = false; // abort if it's not the left button
      emit paintStop( x, y, ok, this );
    }
  d->interndrag = false;
}


void QRPlayField::mouseMove( QMouseEvent* e )
{
  if( d->dragging )
    {
      if( d->interndrag )
	{
	  QPoint	p = ( e->pos() - d->dragorg ) / 16;
	  if( d->gameField->scalingEnabled() )
	    p /= d->gameField->scaleFactor();

	  int	px = d->draglvlorgx - p.x(), py = d->draglvlorgy - p.y();
	  int	pxm = game.tbct.sizeX() - d->gameField->uWidth();
	  int	pym = game.tbct.sizeY() - d->gameField->uHeight();

	  if( px < 0 )
	    px = 0;
	  else if( px > pxm )
	    px = pxm;
	  if( py < 0 )
	    py = 0;
	  else if( py > pym )
	    py = pym;
	  d->gameField->setPos( (unsigned) px, (unsigned) py );
	}
      else if( theQRWin->editMode() )
	{
	  int x, y;
	  bool ok = posFromMouse( e->pos(), x, y );
	  if( x != d->draglvlorgx || y != d->draglvlorgy )
	    {
	      d->draglvlorgx = x;
	      d->draglvlorgy = y;
	      emit paintMove( x, y, ok, this );
	    }
	}
    }
}


bool QRPlayField::posFromMouse( const QPoint & p, int & x, int & y )
{
  if( scalingEnabled() )
    {
      x = (int) ( p.x() / scale() / 32 );
      y = (int) ( p.y() / scale() / 32 );
    }
  else
    {
      x = p.x() / 32;
      y = p.y() / 32;
    }

  x += d->gameField->posX();
  y += d->gameField->posY();
  if( x < 0 || y < 0 || x >= (int) game.tbct.sizeX() 
      || y >= (int) game.tbct.sizeY() )
    return( false );
  return( true );
}


void QRPlayField::paint( const QPoint & p )
{
  int x, y;
  bool ok = posFromMouse( p, x, y );
  emit paintStart( x, y, ok, this );
}


QPoint QRPlayField::posOfElement( int x, int y )
{
  x -= d->gameField->posX();
  y -= d->gameField->posY();
  x *= 32;
  y *= 32;
  QPoint pos( x, y );
  if( d->gameField->scalingEnabled() )
    pos *= d->gameField->scaleFactor();
  return( pos );
}


void QRPlayField::displayFull()
{
  d->gameField->displayFull();
}


RGameField* QRPlayField::viewport()
{
  return( d->gameField );
}


const RGameField* QRPlayField::viewport() const
{
  return( d->gameField );
}


