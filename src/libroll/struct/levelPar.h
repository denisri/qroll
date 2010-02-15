/***************************************************************************
                          levelPar.h  -  description
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


#ifndef QROLL_STRUCT_LEVELPAR_H
#define QROLL_STRUCT_LEVELPAR_H


namespace roll
{

  /**	Paramètres d'en-tete de tableau
   */
  struct LevelPar
  {
    ///	Taille x du tableau
    unsigned char		tx;
    ///	Taille y
    unsigned char		ty;
    ///	Nb de diams nécessaires
    unsigned short	diam;
    ///	Temps
    unsigned short	tps;
    ///	Perméabilité des murs
    unsigned char		perm;
    ///	Vitesse de la bébette
    unsigned char		vit;
    ///	Force roll & bébette
    unsigned char		forc;
    /// Vitesse des générateurs
    unsigned char		vgen;
    /// Couleur des murs/rocks
    unsigned short	cm;
    ///	diams
    unsigned short	cd;
    ///	roll
    unsigned short	cr;
    ///	monstres 1
    unsigned short	cm1;
    ///	herbe
    unsigned short	ch;
    ///	Flags nb de joueurs
    unsigned short	flags;
    ///	Taille max de la bébette
    unsigned short	tbeb;
    ///	Durée mèches des bombes
    unsigned short	dbom;
    ///	Adresse début données tableau
    unsigned short	debu;
  };

}

#endif


