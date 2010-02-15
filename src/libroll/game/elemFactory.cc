/***************************************************************************
                          elemFactory.cc  -  description
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


#include <roll/game/elemFactory.h>
#include <roll/game/roll.h>
#include <roll/game/diam.h>
#include <roll/game/miscEatable.h>
#include <roll/game/key.h>
#include <roll/game/bomb.h>
#include <roll/game/burningBomb.h>
#include <roll/game/lock.h>
#include <roll/game/closeDoor.h>
#include <roll/game/door.h>
#include <roll/game/grassMonster.h>
#include <roll/game/rockMonster.h>
#include <roll/game/diamMonster.h>
#include <roll/game/blob.h>
#include <roll/game/transformWall.h>
#include <roll/game/permeabWall.h>
#include <roll/game/holeWall.h>
#include <roll/game/telepWall.h>
#include <roll/game/keyGen.h>
#include <roll/game/generator.h>
#include <roll/game/conveyorBelt.h>
#include <roll/game/lift.h>
#include <roll/game/spring.h>
#include <roll/game/portcullis.h>
#include <roll/game/ball.h>
#include <roll/game/conveyLever.h>
#include <roll/game/detonator.h>
#include <roll/game/dynamite.h>

#include <roll/game/vars.h>

using namespace roll;


RElemFactory::RElemFactory()
{
}


RElemFactory::~RElemFactory()
{
}


GElem* RElemFactory::createElem( unsigned num )
{
  switch( num )
    {
    case 0:	// monstre 1 (gauche)
    case 1:
    case 2:
    case 40:
    case 41:
    case 42:
    case 80:
    case 81:
    case 82:
    case 120:
    case 121:
    case 122:
    case 160:
    case 161:
    case 162:
    case 200:
    case 201:
    case 202:
    case 6:	// monstre 3
    case 7:
    case 8:
    case 46:
    case 47:
    case 48:
    case 86:
    case 87:
    case 88:
    case 126:
    case 127:
    case 128:
    case 166:
    case 167:
    case 168:
    case 206:
    case 207:
    case 208:
    case 0x100:	// monstres 1 page 1
    case 0x101:
    case 0x102:
    case 0x128:
    case 0x129:
    case 0x12a:
    case 0x150:
    case 0x151:
    case 0x152:
    case 0x178:
    case 0x179:
    case 0x17a:
    case 0x106:	// monstres 3 page 1
    case 0x107:
    case 0x108:
    case 0x12e:
    case 0x12f:
    case 0x130:
      return( new RLeftMonster( num ) );
    case 3:
    case 4:
    case 5:
    case 43:
    case 44:
    case 45:
    case 83:
    case 84:
    case 85:
    case 123:
    case 124:
    case 125:
    case 163:
    case 164:
    case 165:
    case 169:	// monstre 4 bulle
    case 170:
    case 171:
    case 203:
    case 204:
    case 205:
    case 0x103:	// monstre 2 page 1
    case 0x104:
    case 0x105:
    case 0x12b:
    case 0x12c:
    case 0x12d:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x17b:
    case 0x17c:
    case 0x17d:
    case 0x1a3:
    case 0x1a4:
    case 0x1a5:
    case 0x1cb:
    case 0x1cc:
    case 0x1cd:
    case 0x109:	// monstre 4 page 1
    case 0x10a:
    case 0x10b:
      return( new RRightMonster( num ) );
    case 9:	// monstre 4 : glouton bouffe-tout
    case 10:
    case 11:
      return( new RHungryMonster( 9 ) );
    case 49:	// bouffe rock
    case 50:
    case 51:
      return( new RRockMonster( 49 ) );
    case 89:	// bouffe herbe
    case 90:
    case 91:
      return( new RGrassMonster( 89 ) );
    case 129:	// bouffe diams
    case 130:
    case 131:
      return( new RDiamMonster( 129 ) );
    case 209:	// générateur de rocks
    case 210:
    case 211:
      return( new RRockGen( 209 ) );
    case 12:	// bébettes
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 92:
    case 93:
    case 94:
    case 95:
    case 96:
    case 97:
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 137:
    case 172:
    case 173:
    case 174:
    case 175:
    case 176:
    case 177:
    case 212:
    case 213:
    case 214:
    case 215:
    case 216:
    case 217:
    case 0x10c:	// bébettes page 1
    case 0x10d:
    case 0x10e:
    case 0x134:
    case 0x135:
    case 0x136:
    case 0x15c:
    case 0x15d:
    case 0x15e:
    case 0x184:
    case 0x185:
    case 0x186:
    case 0x1ac:
    case 0x1ad:
    case 0x1ae:
    case 0x1d4:
    case 0x1d5:
    case 0x1d6:
      return( new RBlob( num ) );
    case 19:	// herbe
    case 59:
    case 99:
    case 139:
    case 179:
    case 219:
    case 0x112:	// herbes page 1
    case 0x113:
    case 0x13a:
    case 0x13b:
    case 0x162:
    case 0x163:
    case 0x18a:
    case 0x18b:
    case 0x1b2:
    case 0x1b3:
    case 0x1da:
    case 0x1db:
      return( new RGrass( num ) );
    case 20:	// standard walls
    case 21:
    case 22:
    case 31:
    case 60:
    case 61:
    case 62:
    case 71:
    case 100:
    case 101:
    case 102:
    case 111:
    case 140:
    case 141:
    case 142:
    case 151:
    case 180:
    case 181:
    case 182:
    case 191:
    case 220:
    case 221:
    case 222:
    case 231:
    case 0x114:
    case 0x115:
    case 0x13c:
    case 0x13d:
    case 0x164:
    case 0x165:
    case 0x18c:
    case 0x18d:
    case 0x1b4:
    case 0x1b5:
    case 0x1b6:	// (trees)
    case 0x1b7:
    case 0x1b8:
    case 0x1b9:
    case 0x1dc:
    case 0x1dd:
    case 0x1de:	// (trees)
    case 0x1df:
    case 0x1e0:
    case 0x1e1:
      return( new Wall( num ) );
    case 23:	// mur 2 (transformeur)
    case 63:
    case 103:
    case 143:
    case 183:
    case 223:
    case 0x142:
      return( new RTransformWall( num ) );
    case 24:	// mur 3 (passeur)
    case 64:
    case 104:
    case 144:
    case 184:
    case 224:
      return( new RPassingWall( num ) );
    case 25:	// mur 4 (filtreur)
    case 65:
    case 105:
    case 145:
    case 185:
    case 225:
      return( new RPermeabWall( num ) );
    case 26:	// mur téléporteur
    case 0x16a:
      return( new RTelepWall( num ) );
    case 66:	// mur téléporto-transformeur
    case 0x192:
      return( new RTelepTransWall( num ) );
    case 106:	// mur téléporteur à attente
      return( new RTelepPermeabWall( num ) );
    case 27:	// serrure
    case 67:
    case 107:
    case 147:
    case 187:
    case 227:
      return( new RLock( num ) );
    case 30:	// jungle
    case 70:
    case 110:
    case 150:
    case 0x11e:	// jungle page 1
    case 0x146:
    case 0x16e:
    case 0x196:
      return( new RJungle( num ) );
    case 32:	// générateur de monstres 1
      return( new RGenerator( num ) );
    case 72:	// générateur de monstres 2
      return( new RGenerator2( num ) );
    case 112:	// générateur de monstres 3
      return( new RGenerator3( num ) );
    case 152:	// générateur de monstres 4
      return( new RGenerator4( num ) );
    case 192:	// arrivée de téléporteur
      return( new Wall( num ) );
    case 232:	// générateur de clefs
      return( new RKeyGen( num ) );
    case 34:	// roll
    case 35:
    case 74:
    case 75:
    case 114:
    case 115:
    case 154:
    case 155:
    case 194:
    case 195:
    case 234:
    case 235:
      return( new Roll );
    case 28:	// porte fermée
    case 68:
    case 108:
    case 148:
    case 188:
    case 228:
      return( new RCloseDoor( num ) );
    case 29:	// open door
    case 69:
    case 109:
    case 149:
    case 189:
    case 229:
      return( new OpenDoor( num ) );
    case 33:	// bombe éteinte
    case 0x121:	// bombe rouge
    case 0x199:	// bombe rose
      return( new RBomb( num ) );
    case 73:	// bombe allumée
    case 113:
    case 0x149:	// bombe rouge allumée
    case 0x171:
    case 0x1c1:	// bombe rose allumée
    case 0x1e9:
    case 0x14c:	// grenade tombante
    case 0x174:
      {
	unsigned short n = ((num & 0xff) / 40) % 3;
	return( new RBurningBomb( n == 1 ? num : num - 40, n == 1 ? 3 : 2 ) );
      }
    case 153:	// sablier
      return( new RHourglass( num ) );
    case 36:	// clef:
    case 76:
    case 116:
    case 156:
    case 196:
    case 236:
    case 190:	// passe-partout
    case 0x1ec:	// clef spéciale page 1
      return( new RKey( num ) );
    case 37:	// diam
    case 77:
    case 117:
    case 157:
    case 197:
    case 237:
    case 0x125:	// diams page 1
    case 0x14d:
    case 0x175:
    case 0x19d:
    case 0x1c5:
    case 0x1ed:
      return( new RDiam( num ) );
    case 230:	// bonus life
      return( new RBonusLife( num ) );
    case 38:	// rock
    case 78:
    case 118:
    case 158:
    case 198:
    case 238:
    case 0x126:	// rocks page 1
    case 0x14e:
    case 0x176:
    case 0x19e:
    case 0x1c6:
    case 0x1ee:
      return( new Rock( num ) );
    case 39:	// rock tête de mort
    case 79:
    case 119:
    case 159:
      return( new RollingRock( num ) );
    case 199:
      return( new RNitroRock( num ) );
    case 239:
      return( new RNitroPlusRock( num ) );
    case 0x1c7:	// nitro page 1
      return( new Rock( num ) );
    case 0x1ef:	// nitro+ page 1
      return( new RNitroPlusRock( num ) );
    case 193:	// rock bouffe-herbe
      return( new RGrassRock( num ) );
    case 233:	// pastille de force
      return( new RStrengthPill( num ) );
    case 0x110:	// gélule
    case 0x138:
    case 0x160:
    case 0x188:
    case 0x1b0:
    case 0x1d8:
      return( new RFallingEatable( num ) );
    case 0x1bf:	// rock cassable
    case 0x1e7:
      return( new Rock( num ) );
    case 186:	// mur bouffeur
      return( new RHoleWall( num ) );
    case 255:
    case 226:	// illusion
      return( new EmptyElem );

    case 0x1a0:	// tapis roulant 1 (arrêté au départ)
    case 0x1a1:
    case 0x1a2:
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      return( new RConveyorBelt( 0x1a0, 0 ) );
    case 0x156:	// tapis roulant 2 (va à gauche au départ)
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      return( new RConveyorBelt( 0x1a0, 1 ) );
    case 0x157:	// tapis roulant 2 (va à droite au départ)
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      return( new RConveyorBelt( 0x1a0, 2 ) );
    case 0x17f:	// tapis roulant 3 (va à droite au départ, contrôlable)
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      return( new RConveyorBelt( 0x1a0, 3 ) );
    case 0x1a7:	// tapis roulant 4 (arrêté au départ, contrôlable)
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 4 )
	game.tbct.conveyDir.push_back( 0 );
      return( new RConveyorBelt( 0x1a0, 4 ) );
    case 0x1cf:	// tapis roulant 5 (va à gauche, contrôlable)
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 4 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 5 )
	game.tbct.conveyDir.push_back( -1 );
      return( new RConveyorBelt( 0x1a0, 5 ) );
    case 0x17e:	// levier tapis roulant droit
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      return( new RConveyLever( 0x17e, 3 ) );
    case 0x1a6:	// levier tapis roulant immobile
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 4 )
	game.tbct.conveyDir.push_back( 0 );
      return( new RConveyLever( 0x17e, 4 ) );
    case 0x1ce:	// levier tapis roulant gauche
      if( game.tbct.conveyDir.size() == 0 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 1 )
	game.tbct.conveyDir.push_back( -1 );
      if( game.tbct.conveyDir.size() == 2 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 3 )
	game.tbct.conveyDir.push_back( 1 );
      if( game.tbct.conveyDir.size() == 4 )
	game.tbct.conveyDir.push_back( 0 );
      if( game.tbct.conveyDir.size() == 5 )
	game.tbct.conveyDir.push_back( -1 );
      return( new RConveyLever( 0x17e, 5 ) );
    case 0x118:	// balle
      return( new RBall( num, DROI_N ) );
    case 0x140:
      return( new RBall( num-40, GAU_N ) );
    case 0x158:
      return( new RBall( num-80, HAU_N ) );
    case 0x180:
      return( new RBall( num-120, BA_N ) );
    case 0x11b:	// détonateur
    case 0x143:
      return( new RDetonator( 0x11b ) );
    case 0x16b:	// dynamite
    case 0x193:
    case 0x1bb:
      return( new RDynamite( num ) );
    case 0x1be:	// ascenseur "manuel"
      return( new RLift( num ) );
    case 0x1e6:	// ascenseur "automatique"
      return( new RAutoLift( num ) );
    case 0x11c:	// ressort vertical
      return( new RSpring( num, RSpring::VERTICAL ) );
    case 0x144:	// ressort horizontal
      return( new RSpring( num, RSpring::HORIZONTAL ) );
    case 0x195:	// herse ouverte
      return( new EmptyElem );
    case 0x1bd:	// herse ouvrante
      return( new RPortcullis( num + 40, RPortcullis::OPENING ) );
    case 0x1e5:	// herse fermée
      return( new RPortcullis( num, RPortcullis::CLOSED ) );
    default:
      return( new GElem( num ) );
    };
}





