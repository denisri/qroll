/***************************************************************************
                          lock.h  -  description
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


#ifndef ROLL_GAME_LOCK_H
#define ROLL_GAME_LOCK_H


#include <roll/game/eatable.h>


namespace roll
{

  /**	Serrure */
  class RLock : public REatable
  {
  public:
    ///
    RLock( unsigned num ) : REatable( num ) {}
    ///
    virtual ~RLock();

    /**@name	Propriétés de la classe */
    //@{
    ///
    virtual bool isLock() const { return( true ); }
    ///
    virtual RRSoundProcessor::SNDLIST soundWhenEaten() const
    { return( RRSoundProcessor::LOCK ); }
    //@}

    /**@name	Propriétés propres à RLock */
    //@{
    ///	Numéro de type de clef
    virtual unsigned lockNum() const;
    ///	Dit si on peut l'ouvrir par un type de clef particulier
    virtual bool canBeOpened( unsigned key ) const;
    //@}
  };

}

#endif

