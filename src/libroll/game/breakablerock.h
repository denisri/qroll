/***************************************************************************
                          rock.h  -  description
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


#ifndef ROLL_GAME_BREAKABLEROCK_H
#define ROLL_GAME_BREAKABLEROCK_H


#include <roll/game/rock.h>


namespace roll
{

  /**   Rock which breaks when falls */
  class RBreakableRock : public Rock
  {
  public:
    RBreakableRock( unsigned num );
    virtual ~RBreakableRock();

    virtual bool isExplosive() const { return true; }
    virtual bool explodesWhenFalls() const { return true; }
    virtual void doExplode( unsigned x, unsigned y );
    virtual bool squashedBy( const GElem* el ) const
    { return( el->canFall() ); }
    virtual unsigned short nextBreakingElement() const;
    virtual void progressBreak();
  };

}

#endif

