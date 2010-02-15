/***************************************************************************
                          back.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000 by Denis Rivi�re
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


#ifndef ROLL_GAME_BACK_H
#define ROLL_GAME_BACK_H


namespace roll
{
  class GElem;


  /**	El�ments d'arri�re-plan */
  class RBack
  {
  public:
    /**@name	Construction - destruction */
    //@{
    ///
    RBack( unsigned num = 255 );
    ///
    virtual ~RBack();
    //@}

    /**@name	Fonctions */
    //@{
    ///
    virtual unsigned short sprite() const;
    ///	Fond devant ou derri�re l'�l�ment ?
    virtual bool isFront() const { return( false ); }
    ///	Mang� par le roll ? (illusions...)
    virtual bool isEatable() const { return( false ); }
    /**	Foreground element created when a roll leaves this location.
	Default is an EmptyElement */
    virtual GElem* toForeground() const;
    /** ID identifying this particular element - used for packing / saving
	/ transmitting levels by network from elements numbers */
    virtual unsigned short idNumber() const { return( s ); }
    //@}

    /**@name	Donn�es */
    //@{
    ///	Tronche: no de sprite
    unsigned short	s;
    ///	Flags
    unsigned		f;
    //@}

  protected:

  private:
  };


  ///	El�ment de fond, cachant le sprite mouvant (illusion...)
  class RFrontBack : public RBack
  {
  public:
    RFrontBack( unsigned short num = 20 ) : RBack( num ) {}
    virtual ~RFrontBack();
    virtual bool isFront() const { return( true ); }
    /**	Par d�faut, les �l�ments de 1er plans sont mangeables par le Roll, 
	(sinon il dispara�t) */
    virtual bool isEatable() const { return( true ); }
    virtual unsigned short idNumber() const { return( s == 20 ? 226 : s ); }
  };


  /**	Element which creates the same element as background when left by 
	a roll walking through */
  class MirrorBack : public RBack
  {
  public:
    MirrorBack( unsigned short num ) : RBack( num ) {}
    virtual ~MirrorBack();
    virtual GElem* toForeground() const;
  };

}

#endif


