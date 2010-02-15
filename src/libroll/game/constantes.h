/***************************************************************************
                          constantes.h  -  description
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


#ifndef ROLL_GAME_CONSTANTES_H
#define ROLL_GAME_CONSTANTES_H


/**@name	Constantes masques de bits */
//@{

/**@name	Flags caractéristiques des classes d'objets, paquet l1 */
//@{
///
const unsigned	ANIM=1;
///
const unsigned	ANIM_T1=2;
///
const unsigned	ANIM_T2=4;
///
const unsigned	TOMBE=8;
///
const unsigned	TOM_A=16;
///
const unsigned	COMES=32;
///
const unsigned	COM_C=64;
///
const unsigned	COM_T1=128;
///
const unsigned	COM_T2=256;
///
const unsigned	COM_T3=512;
///
const unsigned	EXPL=1024;
///
const unsigned	EXP_CO=0x800;
///
const unsigned	EXP_EC=0x1000;
///
const unsigned	EXP_CH=0x2000;
///
const unsigned	EXP_MS=0x4000;
///
const unsigned	EXP_BA=0x8000;
///
const unsigned	ACTIO=0x10000;
///
const unsigned	ACT_T1=0x20000;
///
const unsigned	ACT_T2=0x40000;
///
const unsigned	ACT_T3=0x80000;
///
const unsigned	ECLA=0x100000;
///
const unsigned	ECL_T1=0x200000;
///
const unsigned	ECL_T2=0x400000;
///
const unsigned	TELEP=0x800000;
///
const unsigned	TEL_T1=0x1000000;
///
const unsigned	TEL_T2=0x2000000;
///
const unsigned	TUE=0x4000000;
///
const unsigned	POUSS=0x8000000;
///
const unsigned	POU_A=0x10000000;
///
const unsigned	FORCE=0x20000000;
///
const unsigned	CASSE=0x40000000;
///
const unsigned	EXPL_TB=0x80000000;
//@}

/**@name	Flags caractéristiques des classes d'objets, paquet l2 */
//@{
///
const unsigned	CASS_E=1;
///
const unsigned	INDES=2;
///
const unsigned	GLISS=4;
///
const unsigned	EXPB=8;
///
const unsigned	BEBCOM=16;
///
const unsigned	EXP_BB=32;
//@}

/**@name	Flags caractéristiques des instances d'objets */
//@{

///
const unsigned	BUSY=1;
///	Demande d'attente par un élément extérieur
const unsigned	WAIT=2;
///	Flag demandé par un élément extérieur
const unsigned	EXTFLG=4;
/**	Comptage de temps après les demandes externes: \\
	une fois WAIT ou EXTFLG mis à 1, REMAIN est mis à 1 à la gestion 
	suivante du sprite en question (ce qui laisse 1 temps pour gérer 
	l'événement demandé). \\
	A la gestion suivante encore tous les flags WAIT, EXTFLG et REMAIN 
	sont remis à zéro.
 */
const unsigned	REMAIN=8;
///
const unsigned	PETE=0x10;
///
const unsigned	RETPET=0x20;
///
const unsigned	TOMB=0x40;
///
const unsigned	ROUL=0x80;
///
const unsigned	ANIMA=0xF00;
///
const unsigned	HAU=0x1000;
///
const unsigned	DROI=0x2000;
///
const unsigned	BA=0x4000;
///
const unsigned	GAU=0x8000;
///
const unsigned	HAU_N=0x30000;
///
const unsigned	DROI_N=0xC0000;
///
const unsigned	BA_N=0x300000;
///
const unsigned	GAU_N=0xC00000;
///
const unsigned	GERE=0x1000000;
///
const unsigned	MONT=0x2000000;
///
const unsigned	ONOFF=0x4000000;
///
const unsigned	ETEND=0x8000000;
///
const unsigned	INDE=0x10000000;
///
const unsigned	EXP1=0x20000000;
///
const unsigned	EXP2=0x40000000;

/**@name	Combinaisons de plusiseurs flags */
//@{
///
const unsigned	BOUGE = HAU | BA | GAU | DROI;
///
const unsigned	BOUGE_N = HAU_N | BA_N | GAU_N | DROI_N;
//@}
//@}

/**@name	Flags des objets de fond (fixes, passifs) */
//@{

/**@name	Classes d'objets */
//@{
///	Animé
const unsigned	BC_ANIM = 0x1;
///	Type d'animation
const unsigned	BC_ANIM_T1 = 0x2;
///	Type d'animation
const unsigned	BC_ANIM_T2 = 0x4;
///	Opaque: apparait par dessus le plan principal
const unsigned	BC_OPAQ = 0x8;
///	Creux: traversable, pour Roll
const unsigned	BC_CREU_RL = 0x10;
///	Creux pour monstre
const unsigned	BC_CREU_MS = 0x20;
///	Creux pour rocher
const unsigned	BC_CREU_RC = 0x40;
///	Creux pour bébette
const unsigned	BC_CREU_BB = 0x80;
///	Comestible
const unsigned	BC_COMES = 0x100;
///	Comestible pour ??
const unsigned	BC_COM_C = 0x200;
///
const unsigned	BC_COM_T1 = 0x400;
///
const unsigned	BC_COM_T2 = 0x800;
///
const unsigned	BC_COM_T3 = 0x1000;
///	Actionnable
const unsigned	BC_ACTIO = 0x2000;
///	Actionnable, types
const unsigned	BC_ACT_T1 = 0x4000;
///
const unsigned	BC_ACT_T2 = 0x8000;
///
const unsigned	BC_ACT_T3 = 0x10000;
///	classe indestructible
const unsigned	BC_INDES = 0x20000;
//@}

/**@name	Instances */
//@{
///
const unsigned	BI_BEBCOM = 0x1;
///	Ne pas gérer ce tour
const unsigned	BI_BUSY = 0x2;
///	Illusion (~creux et opaque)
const unsigned	BI_ILLU = 0x4;
///	Offset d'animation (pour la désynchro)
const unsigned	BI_ANIMA = 0x8;
///	Déjà géré
const unsigned	BI_GERE = 0x10;
///	?
const unsigned	BI_ONOFF = 0x20;
///	Indestructible (instance)
const unsigned	BI_INDE = 0x40;
//@}

//@}

//@}



#endif



