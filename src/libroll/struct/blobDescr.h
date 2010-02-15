/***************************************************************************
                          blobdescr.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivière
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

  /**	Descripteurs de bébettes
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
	2 : transfo 2: bloquée
	3 : pas de transfo du tout (bébette dans un bord)
    */
    unsigned short	flg;
    ///	Flag bébette bloquée
    unsigned short	blk;
    ///	Flag bébette existe ( ???? ) ( existait depuis le début ?? )
    unsigned short	exi;
    ///	Données transformation 1
    unsigned short	d1[3][3];
    ///	Données transformation 2
    unsigned short	d2[3][3];
  };

}


#endif


