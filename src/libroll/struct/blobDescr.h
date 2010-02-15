/***************************************************************************
                          blobdescr.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivi�re
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

#ifndef ROLL_STRUCT_BLOBDESCR_H
#define ROLL_STRUCT_BLOBDESCR_H


namespace roll
{

  /**	Descripteurs de b�bettes
   */
  class BlobDescr
  {
  public:
    BlobDescr() {}
    ~BlobDescr() {}

    BlobDescr & operator = ( BlobDescr & beb );

    unsigned short	tmax;
    unsigned short	cpt;
    /**	Flag de transformation: \\
	0 : rien
	1 : transfo 1: trop grosse
	2 : transfo 2: bloqu�e
	3 : pas de transfo du tout (b�bette dans un bord)
    */
    unsigned short	flg;
    ///	Flag b�bette bloqu�e
    unsigned short	blk;
    ///	Flag b�bette existe ( ???? ) ( existait depuis le d�but ?? )
    unsigned short	exi;
    ///	Donn�es transformation 1
    unsigned short	d1[3][3];
    ///	Donn�es transformation 2
    unsigned short	d2[3][3];
  };

}


#endif


