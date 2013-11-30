/***************************************************************************
                          roll.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2007 by Denis Riviere
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


#include <roll/game/roll.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <roll/game/diam.h>
#include <roll/game/key.h>
#include <roll/game/lock.h>
#include <roll/game/bomb.h>
#include <roll/game/portcullis.h>
#include <roll/game/openPortcullis.h>
#include <roll/game/ball.h>
#include <roll/game/button.h>
#include <roll/game/playerServer.h>
#include <roll/sound/soundProcessor.h>
#include <roll/game/random.h>

using namespace roll;


Roll::Roll( unsigned num ) : GElem( num ), _num( 0 ), _backRoll( 0 )
{
  //cout << "Roll created\n";
}


Roll::~Roll()
{
  delete _backRoll;
}


void Roll::realProcess( unsigned x, unsigned y )
{
  PlayerServer::iterator  ip = game.tbct.players->find( _num );

  if( ip == game.tbct.players->end() )
  {
    replaceAndProcessBack( x, y );
    delete this;	// j'existe plus, je m'efface.
    return;
  }

  Player	& pl = (*ip).second;

  if( pl.win )
  {
    replaceAndProcessBack( x, y );
    delete this;
    return;
  }

  pl.pos = Coord( x, y );
  /* already tested
  if( pl.win )
  {
    f &= ~( GAU | DROI | HAU | BA );
    return;
  }
  */

  pl.dead = 0;
  //	out << "Roll " << _num << "vivant\n";

  f &= ~(HAU | DROI | BA | GAU);

  if( game.tbct.frozen ) return;	// frozen game: don't move

  if( pl.kbd == game.K_ABORT 
      || (sp_flg[game.tbct.d[x-1][y]->s].l1 & TUE)
      || (sp_flg[game.tbct.d[x+1][y]->s].l1 & TUE)
      || (sp_flg[game.tbct.d[x][y-1]->s].l1 & TUE)
      || (sp_flg[game.tbct.d[x][y+1]->s].l1 & TUE) )
    f |= PETE;

  else if( pl.kbd == game.K_BOMB && pl.launchedBomb == -1 )
  {
    unsigned	& nbb = pl.bombs[ pl.bombToLaunch ];
    if( nbb > 0 )
    {
      --nbb;
      pl.launchedBomb = pl.bombToLaunch;
    }
  }

  unsigned	spdc = _num ? 0x100 : 0;

  switch( pl.kbd )
  {
  case Game::K_LEFT:	// gauche
    pl.latency = 0;
    s = 34 + 3*40 + spdc;

    if( poss( x-1, y, -1, _num ) && !pl.fire )
    {
      delete game.tbct.d[x-1][y];
      game.tbct.d[x-1][y] = this;
      //game.tbct.d[x][y] = elemBehind( _num );
      //_backRoll = 0;
      f = (f & ~RETPET) | GAU | GAU_N;
      // prend un truc sur le coin de la gueule ?
      GElem	*elup = game.tbct.d[x-1][y-1];
      if( elup->isFalling() && squashedBy( elup ) ) f |= PETE;
      replaceAndProcessBack( x, y );
    }
    else if( f & RETPET )
      f |= PETE;
    break;

  case Game::K_RIGHT:	// droite
    pl.latency = 0;
    s = 35 + 3*40 + spdc;

    if( poss( x+1, y, 1, _num ) && !pl.fire )
    {
      delete game.tbct.d[x+1][y];
      game.tbct.d[x+1][y] = this;
      //game.tbct.d[x][y] = elemBehind( _num );
      //_backRoll = 0;
      f = (f & ~RETPET) | BUSY | DROI | DROI_N;
      // prend un truc sur le coin de la gueule ?
      GElem	*elup = game.tbct.d[x+1][y-1];
      if( elup->isFalling() && squashedBy( elup ) ) f |= PETE;
      replaceAndProcessBack( x, y );
    }
    else if( f & RETPET )
      f |= PETE;
    break;

  case Game::K_UP:		// haut
    if( f & RETPET )
      f |= PETE;
    if( pl.latency )
    {
      s = 34 + spdc;
      pl.latency = 0;
    }

    if( poss( x, y-1, 0, _num ) && !pl.fire )
    {
      delete game.tbct.d[x][y-1];
      game.tbct.d[x][y-1] = this;
      //game.tbct.d[x][y] = elemBehind( _num );
      //_backRoll = 0;
      f |= HAU | HAU_N;
      // prend un truc sur le coin de la gueule ?
      if( y > 1 )
      {
        GElem	*elup = game.tbct.d[x][y-2];
        if( elup->isFalling() && squashedBy( elup ) ) f |= PETE;
      }
      replaceAndProcessBack( x, y );
    }
    break;

  case Game::K_DOWN:		// bas
    if( pl.latency )
    {
      s = 34 + spdc;
      pl.latency = 0;
    }

    if( poss( x, y+1, 0, _num ) & !pl.fire )
    {
      delete game.tbct.d[x][y+1];
      game.tbct.d[x][y+1] = this;
      //game.tbct.d[x][y] = elemBehind( _num );
      //_backRoll = 0;
      f = (f & ~RETPET) | BUSY | BA | BA_N;
      replaceAndProcessBack( x, y );
      game.tbct.d[x][y]->f &= ~TOMB;	// prevent scratch
    }
    else if( f & RETPET ) 
      f |= PETE;
    break;

  default:		// pas de mouvement
    RRSoundProcessor::processor().stop( RollSoundBank::ROLL_PUSH );

    if( f & RETPET ) f |= PETE;
    if( s != 115+spdc )	//pousseur
    {
      if( s != 35+spdc && s != 75+spdc )
      {
        s = 34 + spdc;
        ++pl.latency;
        if( pl.latency >= 50 ) s = 35 + spdc;
      }
      else	// yawner
      {
        ++pl.latency;
        if( pl.latency == 55 )
        {
          s = 75 + spdc;
          RRSoundProcessor::processor().process( RollSoundBank::ROLL_YAWN );
        }
        if( pl.latency == 60 ) s = 35 + spdc;
        if( pl.latency > 62 )
        {	//	end of yawn
          pl.latency = 0;
          s = 34 + spdc;
        }
      }
    }
    if( _backRoll )
      _backRoll->process( x, y );
    break;
  }	// fin switch */
}


bool Roll::poss( unsigned i, unsigned j, int pouss, int mec )
{
  unsigned long		flag, busy;
  unsigned short	spr;
  bool			ok = true;
  div_t			sp;
  GElem			*& enear = *(&game.tbct.d[i][j]);
  RBack			*& nback = *(&game.tbct.b[i][j]);
  Player		& pl = (*game.tbct.players)[ mec ];
  bool			fire = pl.fire;
  ROpenPortcullis	*op;

  if( nback->isEatable() )
    {		// manger illusion
      delete nback;
      nback = backFactory.createBack( 255 );
    }

  if( fire && enear->isEmpty() )
    {
      if( pl.ball )	// ball to launch
	{
	  bool		busy;
	  unsigned	dir;

	  if( j < pl.pos.y )
	    {
	      busy = false;
	      dir = DROI_N;
	    }
	  else if( i < pl.pos.x )
	    {
	      busy = false;
	      dir = HAU_N;
	    }
	  else if( j > pl.pos.y )
	    {
	      busy = true;
	      dir = GAU_N;
	    }
	  else
	    {
	      busy = true;
	      dir = BA_N;
	    }
	  delete enear;
	  enear = new RBall( 0x118, dir, true );
	  if( busy )
	    enear->f |= BUSY;
	  pl.ball = false;
	  return( false );
	}
      else if( ( op = dynamic_cast<ROpenPortcullis *>( nback ) ) )
	{
	  //cout << "herse... fermeture ( " << op->s << " )\n";
	  delete enear;
	  enear = new RPortcullis( op->s + 80, RPortcullis::CLOSING );
	  if( j > pl.pos.y || ( j == pl.pos.y && i > pl.pos.x ) )
	    enear->f |= BUSY;
	  delete nback;
	  nback = new RBack( 255 );
	  return( false );
	}
    }

  flag = sp_flg[ spr=enear->s ].l1;

  if( enear->isEatable() && ( !enear->isMoving() || pl.fire == 0 ) )
    {
      sp = div( spr & 0xff, 40 );
      RPortcullis	*pc;

      if( enear->isDiam() )	// diam
	{
	  pl.score += ((RDiam *)enear)->diamNum() + 1;
	  if( game.tbct.diams() > 0 )
	    {
	      game.tbct.setDiams( game.tbct.diams() - 1 );
	      if( game.tbct.diams() == 0 )
		game.tbct.doorOpen = true;
	    }
	}

      else if( enear->isDoor() )	// porte ouverte
	pl.win = 1;

      else if( enear->isKey() )	// clef
	{
	  pl.key = ((RKey *) enear)->keyNum();
	}

      else if( enear->isLock() )	// serrure
	{
	  if( pl.key >= 0 
	      && ((RLock *)enear)->canBeOpened( (unsigned) pl.key ) )
	    {	//	ouvrable
	      pl.key = -1;	// no more key
	    }
	  else	//	pas ouvrable
	    ok = false;
	}

      else if( enear->isJungle() ) // jungle
	{
	  if( (rrand() & 0xff) > game.tbct.strength() )	// jet de force
	    ok = false;		// rate
        }

      else if( enear->isBomb() )	// bombe
	{
	  ++pl.bombs[((RBomb*) enear)->bombNum()];
	}

      else if( enear->isBonusLife() )	// bonus life
	{
	  ++pl.lives;
	  game.lifeGained = 1;
	}

      else if( enear->isStrengthPill() )	// pastille de force
	{
	  unsigned stg = game.tbct.strength() + 10;
	  game.tbct.setStrength( (stg > 255) ? 255 : stg );
	}

      else if( enear->isHourglass() )	// sablier
	{
	  game.tbct.setTime( game.tbct.time() + 10 );
	  game.timeChanged = true;
	}

      else if( (pc = dynamic_cast<RPortcullis *>( enear ) ) )
	{
	  ok = false;
	  //cout << "herse ( " << pc->s << " )...";
	  if( fire && pc->state() == RPortcullis::CLOSED )
	    {
	      pc->setState( RPortcullis::OPENING );
	      if( i > pl.pos.x || j > pl.pos.y )
		pc->f |= BUSY;
	      //cout << "ouverture\n";
	    }
	  /*else
	    cout << "rien\n";*/
	  return( false );
	}

      else if( enear->isBall() )	// balle
	{
	  if( pl.ball )
	    ok = false;		// you've got one already...
	  else
	    //cout << "prends baballe\n";
	    pl.ball = true;
	}
      else if( enear->isButton() )	// bouton / levier
	{
	  ok = false;
	  if( fire )
	    ((RButton *) enear)->activate( i, j );
	}

      RRSoundProcessor::processor().stop( RollSoundBank::ROLL_PUSH );
      if( ok )
	{
	  RRSoundProcessor::processor().process( enear->soundWhenEaten() );
	  delete enear;
	  enear = nback->toForeground();
	  return( true );
	}
      else return( false );
    }

  //	Pousse

  if( pouss == 0 || ( pouss < 0 && i == 0 ) 
      || ( pouss > 0 && i == game.tbct.sizeX() - 1 ) )
    return false; // not pusheable

  GElem	*& efar = * (&game.tbct.d[i+pouss][j]);

  if( pouss != 0 && (flag & POUSS) && (efar->s==255)
      && !(game.tbct.d[i+pouss][j-1]->f & ROUL) )
    {
      RRSoundProcessor::processor().processIfNotUsed( RollSoundBank::ROLL_PUSH );
      if( (rrand() & 0xff) < game.tbct.strength() )
	{
	  /*if( nback->s == 20 )	// illusion
	    {
	      delete nback;
	      nback = new RFrontBack( 20 );
	      }*/

	  // pushed object doesn't fall anymore
	  enear->setFalling( false );
	  RFallingElem	*fel = dynamic_cast<RFallingElem *>( enear );
	  if( fel )
	    {
	      if( pouss < 0 )
		fel->moveLeft( i, j );
	      else
		fel->moveRight( i, j );
	    }
	  else
	    {
	      delete efar;
	      efar = enear;
	      enear = nback->toForeground();
	      if( pouss > 0 ) busy = BUSY;
	      else busy = 0;
	      if( pouss < 0 ) flag = GAU;
	      else flag = DROI;
	      efar->f |= flag | ROUL | busy;
	    }
	  return( true );
	}
    }
  else RRSoundProcessor::processor().stop( RollSoundBank::ROLL_PUSH );

  return( false );
}


GElem* Roll::elemBehind( unsigned mec, unsigned x, unsigned y )
{
  PlayerServer::iterator ir = game.tbct.players->find( mec );
  if( ir == game.tbct.players->end() )
  {
    // this roll has been removed
    return game.tbct.b[x][y]->toForeground();
  }

  Player	& pl = ir->second;
  int		& bomb = pl.launchedBomb;
  int		copy = bomb;

  if( _backRoll )
    return( _backRoll );

  if( bomb >= 0 )
    {
      bomb = -1;
      GElem	*el = elFactory.createElem( RBomb::burningBomb( copy ) );
      if( el->canFall() )
	el->f |= TOMB;
      return( el );
    }
  else return( game.tbct.b[x][y]->toForeground() );
}


Roll* Roll::frontRoll( unsigned x, unsigned y ) const
{
  GElem	*gel = game.tbct.d[x][y];
  Roll	*rl = dynamic_cast<Roll *>( gel );
  Roll	*rl2;

  if( !rl )
    return( 0 );
  for( rl2 = rl->backRoll(); rl2 && rl2!= this; 
       rl = rl2, rl2 = rl->backRoll() );

  return( rl2 ? rl : 0 );
}


void Roll::replaceAndProcessBack( unsigned x, unsigned y )
{
  Roll	*front = frontRoll( x, y );

  //	replace
  if( front )
    front->setBackRoll( _backRoll );
  else
    game.tbct.d[x][y] = elemBehind( _num, x, y );

  //	process back
  if( _backRoll )
    _backRoll->process( x, y );

  _backRoll = 0;
}

