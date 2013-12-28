/***************************************************************************
                          gElem.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivière
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


#include <roll/game/gElem.h>
#include <roll/game/vars.h>
#include <roll/game/constantes.h>
#include <roll/sound/soundProcessor.h>

using namespace roll;


unsigned short		roll::anim = 0;


GElem::~GElem()
{
}


bool GElem::isAnimated() const
{
  return( s != 255 && (sp_flg[s].l1 & ANIM) );
}


GElem::AnimType GElem::animType() const
{
  if( !isAnimated() )
    return( AnimNone );
  return( (AnimType) ( ( (sp_flg[s].l1 & (ANIM_T1 | ANIM_T2)) >> 1 ) + 1 ) );
}


unsigned short GElem::animation() const
{
  unsigned short	spr = s, ani;

  if( isAnimated() )
    {
      ani = anim + ( ( f & ANIMA ) >> 8 );
      switch( animType() )
	{
	case AnimStraight:
	  spr += ani % 3;
	  break;
	case AnimReturn:
	  ani &= 3;
	  if( ani == 3 ) ani = 1;
	  spr += ani;
	  break;
	case AnimVertical:
	  ani &= 3;
	  if( ani == 3 ) ani = 1;
	  spr += ani*40;
	  break;
	default:
	  break;
	}
    }
  return( spr );
}


unsigned short GElem::backAnim( RBack* back ) const
{
  return( back->s );
}


unsigned short GElem::sprite( RBack* back ) const
{
  unsigned short	spr;

  //	background prioritaire ?
  if( back && back->isFront() )
    {
      spr = back->sprite();
      if( spr != 255 )
        return( spr );
    }	// pour l'instant...

  //	explosion ?
  if( f & EXP1 )
    return( 18 );
  if( f & EXP2 )
    return( 58 );

  return( animation() );
}


unsigned short GElem::movingSprite() const
{
  if( f & BOUGE )
    return( animation() );

  else return( 255 );
}


unsigned short GElem::backStillSprite( RBack* back ) const
{
  if( back->isFront() || (f & (EXP1 | EXP2)) )
    return( 255 );	// pour l'instant

  if( !(f & BOUGE) )
    return( animation() );
  else if( !back->isFront() )
    return( back->sprite() );
  else return( 255 );
}


unsigned short GElem::frontStillSprite( RBack* back ) const
{
  if( back->isFront() )
    return( back->s );

  if( f & EXP1 )
    return( 18 );
  else if( f & EXP2 )
    return( 58 );

  else return( 255 );
}


void GElem::process( unsigned x, unsigned y )
{
  if( f & BUSY )
  {
    f &= ~BUSY;
  }
  else
  {
    if( f & EXP1 )			// explosion
    {
      f = ( f & ~EXP1 ) | EXP2;	// change d'explosion
      return;
    }
    if( f & EXP2 )
      f &= ~EXP2;			// dernière attente avant explosion
    if( f & PETE )			// explosion à créer
    {
      explode( x, y );		// ben oui...
      return;
    }

    if( f & REMAIN )			// retenait qqchose ?
      f &= ~(REMAIN | WAIT | EXTFLG);	// alors oublie maintenant.
    if( f & (WAIT | EXTFLG) )		// qqch à retenir ?
      f |= REMAIN;

    if( !(f & WAIT) )			// fallait pas attendre ?
      realProcess( x, y );		// partie différenciée
  }
}


void GElem::realProcess( unsigned, unsigned )
{
}


void GElem::doExplode( unsigned x, unsigned y )
{
  int                   i,j;
  unsigned long         fl;

  if( isExplosive() )
  {
    if( x == 0 || y == 0 || x+1 == game.tbct.sizeX() 
        || y+1 == game.tbct.sizeY() )
      return;   // au bord, on ne pète pas.

    for( j=0; j<3; ++j ) for( i=0; i<3; ++i )
    {
      RBack *&bk = game.tbct.b[x+i-1][y+j-1];
      if( !(sp_flg[bk->s].l2 & BC_INDES) && !(bk->f & BI_INDE) )
        // background destructible
      {
        //      change background
        delete bk;
        bk = backFactory.createBack( explo.d[explo.a[s]][i][j] );

        GElem *& ge = game.tbct.d[x+i-1][y+j-1];

        if( !(sp_flg[ge->s].l2 & INDES) && !(ge->f & INDE) ) // 1er plan
        {
          if( i+j*3 > 4 ) fl = BUSY | EXP1;
          else fl = EXP1;
          if( (i!=1 || j!=1) && ge->explodesWhenBlown() )
            ge->f |= PETE | fl;
          else
          {
            delete ge;
            ge = elFactory.createElem( explo.d[explo.a[s]][i][j] );
            ge->f |= fl;
            if( ge->chainExplodes() )
              ge->f |= PETE;
          }
        }
      }
    }
    RRSoundProcessor::processor().process( RollSoundBank::EXPLO );
  }
}


void GElem::explode( unsigned x, unsigned y )
{
  GElem	*el = game.tbct.d[x][y];
  if( el )
    el->doExplode( x, y );
}


GElem* GElem::transformed() const
{
  return( elFactory.createElem( s ) );
}


bool GElem::isSlippery() const
{
  return( ( sp_flg[ s ].l2 & GLISS ) != 0 );
}


bool GElem::canBeInBorder() const
{
  return( false );
}


bool GElem::canBePushed() const
{
  return ( sp_flg[s].l1 & POUSS ) != 0;
}



//	Vide


unsigned short EmptyElem::sprite( RBack* back ) const
{
  unsigned short	spr = GElem::sprite( back );
  if( spr == s && back )
    return( back->sprite() );
  else
    return( spr );
}


unsigned short EmptyElem::backStillSprite( RBack* back ) const
{
  unsigned short	spr = GElem::backStillSprite( back );
  if( spr == s && back )
    return( back->sprite() );
  else
    return( spr );
}


