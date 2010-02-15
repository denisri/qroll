/***************************************************************************
                          fallingEatable.h  -  description
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


#ifndef ROLL_GAME_FALLINGEATABLE_H
#define ROLL_GAME_FALLINGEATABLE_H


#include <roll/game/rock.h>


namespace roll
{

  /**	Chose qui se mange et qui tombe */
  class RFallingEatable : public RFallingElem
  {
  public:
    RFallingEatable( unsigned num ) : RFallingElem( num ) {}
    virtual ~RFallingEatable();
    virtual bool isEatable() const { return( true ); }
    virtual RRSoundProcessor::SNDLIST soundWhenFalls() const
    { return( RRSoundProcessor::DIAM ); }
  };

}

#endif
