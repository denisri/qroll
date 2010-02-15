/***************************************************************************
                          soundProcessor.h
                             -------------------
    begin                : 1999
    copyright            : (C) 1999-2004 by Denis Rivière
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


#ifndef ROLL_SOUND_SOUNDPROCESSOR_H
#define ROLL_SOUND_SOUNDPROCESSOR_H

#include <string>


/**	Processeur de sons & bruitages.

	Particularité :

	- intarface portable, indépendante du système et kit de son utilisé.
	- dedans, ça peut dépendre (pour l'instant: Linux DSP / Sun audio / 
          QSound)

	Comme les sources dépendant d'une librairie de sons, on utilise dans 
	les Makefile des macros pour savoir si on compile la partie son 
	spécifique ou non.

	Si la partie son n'est pas compilée, l'interface existe quand-même, 
	ça ne fera pas de bruit, c'est tout.

	La classe est dérivable pour brancher les différentes implémentations, 
	utilisant différents 'soundkits'.

	Pour l'instant, les macros reconnues sont:

	RR_DSP : partie utilisant directement /dev/dsp, classe RRSdpSound, 
	option de compil -DRR_DSP.
 */
class RRSoundProcessor
{
public:
  enum SNDLIST
  {
    BALL, 
    BLOB, 
    BOMB, 
    BONUSLIFE, 
    CONVEYLEVER, 
    CONVEYORBELT, 
    DIAM, 
    DOOR, 
    EXPLO, 
    GRASS, 
    HUNGRY, 
    KEY, 
    KEY_FALL, 
    LIFT, 
    LOCK, 
    ROCK, 
    ROLL_PUSH, 
    ROLL_YAWN, 
    SPRING, 
    STRENGTH, 
    TIME, 
    DONG1, 
    DONG2, 
    DONG3, 
    DONG4, 
    DONG5, 
    DONG6, 
    DONG7, 
    DONG8, 
    DONG9, 
    NO_SOUND
  };
  RRSoundProcessor();
  virtual ~RRSoundProcessor();
  ///	Commence à jouer un bruitage
  virtual void process( SNDLIST type );
  virtual unsigned inuse( SNDLIST type )
    { return( _inuse[type] ); }
  virtual void processIfNotUsed( SNDLIST type )
    { if( type < NO_SOUND && !inuse( type ) ) process( type ); }
  virtual void stop() {}
  virtual void stop( SNDLIST type );
  ///	Ferme la sortie son (->devient muet)
  virtual void close() {}
  ///	Désactive le son
  virtual void disable() { close(); }
  ///	Réactive le son
  virtual void enable() { disable(); init(); }
  ///	Dit si le processeur marche
  virtual bool isOK() const { return( false ); }
  virtual std::string name() const { return "no sound"; }
  /**	Used to choose between several working sound processors.
     It should be related to a quality evaluation because when several 
     processors are available, you want to select the best one.
     The base RRSoundProcessor has priority 0, and is mute so it will 
     normally have the lowest priority.
   */
  virtual float priorityRating() const { return 0; }

  static RRSoundProcessor & processor();

protected:
  static void registerProcessor( RRSoundProcessor* p );
  static void unregisterProcessor( RRSoundProcessor* p );

  ///	tableau des nb de sons en cours
  unsigned	_inuse[ NO_SOUND ];
  ///	table des noms de fichiers sons
  std::string	_sndFile[ NO_SOUND ];
  virtual void init();
};


inline void RRSoundProcessor::stop( SNDLIST ) {}


#endif
