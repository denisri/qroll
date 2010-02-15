/***************************************************************************
                          seriesAtari.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¨re
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


#include <roll/struct/seriesAtari.h>
#include <roll/game/vars.h>
#include <roll/struct/workLevel.h>
#include <roll/struct/simpleLevel.h>
#include <roll/struct/levelAtari.h>

using namespace roll;
using namespace std;


SeriesAtari::SeriesAtari() : Series(), nt( 0 ), fnt( 0 ), tb( 0 )
{
}


SeriesAtari::~SeriesAtari()
{
  if( fnt ) delete[] fnt;
  if( tb ) delete[] tb;
}


SeriesAtari::SeriesAtari( const string & name )
  : Series( name ), fnt( 0 ), tb( 0 )
{
  load( name );
}


bool SeriesAtari::load( const string & name )
{
  int i;

  out << "Loading series " << name << "\n";

  ifstream	fich( name.c_str(), ios::in | ios::binary );

  if( !fich )
    {
      out << name << " not found\n";
      return( false );
    }

  fich.unsetf( ios::skipws );

  unsigned char	c1,c2,c3,c4;
  unsigned long	dec = 0;

  unsigned short	dm, ex, bb, vl;
  unsigned long		cd;

  fich >> c1 >> c2;
  dm = ((unsigned short) c1<<8) + c2;
  if( dm >= 1000 )
    return( false ); // probably wrong format
  fich >> c1 >> c2;
  ex = ((unsigned short) c1<<8) + c2;
  if( ex >= 1000 )
    return( false );
  fich >> c1 >> c2;
  bb=((unsigned short)c1<<8)+c2;
  if( bb >= 1000 )
    return( false );
  fich >> c1 >> c2;
  vl=((unsigned short)c1<<8)+c2;
  if( vl > 5 )
    return( false );
  fich >> c1 >> c2;
  cd=((unsigned long)c1<<8)+c2;
  fich >> c1 >> c2;
  cd+=((unsigned long)c1<<24)+((unsigned long)c2<<16);
  if( cd >= 10000 )
    return( false );

  //cout << "elements map :\n";
  unsigned short	tspr[240];
  for(i=0; i<240; i++)
    {
      fich >> c1 >> c2;
      if( c1 > 1 )
	return( false );
      if(c1!=0) c1=1;
      tspr[i]=((unsigned short)c1<<8)+c2;
      //cout << tspr[i] << " ";
    }
  //cout << endl;

  fich >> c1 >> c2;
  unsigned short	tnt=((unsigned short)c1<<8)+c2;
  if( tnt > 1000 )
    return( false );
  out << "\nLevels : " << tnt << "\n\n";

  vector<unsigned long>	tfnt( tnt );

  for( i=0; i<tnt; i++ )
    {
      fich >> c1 >> c2 >> c3 >> c4;
      tfnt[i] = ((unsigned long)c1<<24) | ((unsigned long)c2<<16)
	| ((unsigned long)c3<<8) | (unsigned long)c4;
      if( i==0 ) dec = tfnt[0];
      tfnt[i] -= dec;
      if( i>0 && tfnt[i]<=tfnt[i-1] )
	{
	  /*out << "fnt[" << (i-1) << "]: " << tfnt[i-1]
	      << " >= fnt[" << i << "]: " << tfnt[i]
	      << ". cars: "
	      << (int)c1 << " " << (int)c2 << " "
	      << (int)c3 << " " << (int)c4
	      << "\n";*/
	  return( false );
	}
    }

  if( !fich )
    return( false );

  //out << "Dec : " << dec << "\n";
  //out << "\n";
  /*for( i=0; i<tnt; i++ )
    out << i << " : " << tfnt[i] << "\n";
    out << "\n";*/

  // file seems OK up to now, swapping to real data
  diam = dm;
  expl = ex;
  beb = bb;
  val = vl;
  cod = cd;
  nt = tnt;
  memcpy( spr, tspr, 240 * sizeof( unsigned short ) );

  if( fnt ) delete[] fnt;
  fnt = new unsigned long[ nt ];
  memcpy( fnt, &tfnt[0], nt * sizeof( unsigned long ) );
  if( tb ) delete[] tb;
  tb = new LevelAtari[nt];

  for( i=0; i<nt; i++ )
    {
      tb[i].load( fich );
      //out << "Tableau " << i << " : taille : " << tb[i].size << "\n";
    }

  setFilename( name );
  convList();
  setModified( false );

  return( true );
}


void SeriesAtari::convList()
{
  unsigned short	i,j,sp,s,s2;
  unsigned char		sprconv[480];
  unsigned char		spr_on[480];

  for( i=0; i<480; i++ )
    {
      sprconv[i] = 0xFF;
      spr_on[i] = 0;
    }

  /*	Bonhomme	*/

  for(i=0; i<6; i++)
    {
      spr_on[34+i*40]=0xFF;
      spr_on[35+i*40]=0xFF;
      sprconv[34+i*40]=i*2;
      sprconv[35+i*40]=i*2+1;
    }

  spr_on[18]=0xFF;
  spr_on[58]=0xFF;
  sprconv[18]=12;
  sprconv[58]=13;

  sp=14;
  for(i=0; i<240; i++)
    {
      s=spr[i];
      if(s!=0xFF)
	{
	  if(s>=0x100) s-=16;
	  s2=s;
	  if(!spr_on[s]) for(j=0; j<(spr_link[2*s] & 0x7F); j++)
	    {
	      spr_on[s2]=0xFF;
	      sprconv[s2]=sp++;
	      s2=spr_link[2*s2+1];
	      if(s>=240) s2+=240;
	    }
	}
    }

  for(i=0; i<240; i++) spr[i]=0xFF;

  for(i=0; i<480; i++) if(spr_on[i])
    {
      if(i<240) spr[sprconv[i]]=i;
      else spr[sprconv[i]]=i+16;
    }
}


bool SeriesAtari::pack( const SimpleLevel &, unsigned )
{
  return( false );	// can't pack for now...
}


void SeriesAtari::size( unsigned level, unsigned & sx, unsigned & sy ) const
{
  sx = tb[level].tx;
  sy = tb[level].ty;
}


void SeriesAtari::unpack( unsigned level, SimpleLevel & sl ) const
{
  resizeSimpleLevel( level, sl );
  unsigned		i, sz = sl.sizeX() * sl.sizeY();
  const LevelAtari	& lvl = tb[level];
  unsigned short	elem;
  unsigned short	*fg = sl.foreground();

  sl._diams          = lvl.diams();
  sl._time           = lvl.time();
  sl._permeability   = lvl.permeability();
  sl._speed          = lvl.speed();
  sl._strength       = lvl.strength();
  sl._genspeed       = lvl.genSpeed();
  sl._blobmaxsize    = lvl.blobMaxSize();
  sl._bombdelay      = lvl.bombDelay();
  sl._colwall	     = lvl.wallColor();
  sl._coldiam	     = lvl.diamColor();
  sl._colroll	     = lvl.rollColor();
  sl._colleftmonster = lvl.leftMonsterColor();
  sl._colgrass	     = lvl.grassColor();
  sl._flags	     = lvl.flags();

  // level data

  for( i=0; i<sz; ++i )
    {
      elem = lvl.data[ i ];
      if( elem != 255 )
	elem = spr[ elem ];
      fg[ i ] = elem;
    }

  // variable parameters

  i=0;
  unsigned short	pa = 1, nr, adr, j, k;
  LevelParam::ParamCode	code;

  while( pa!=0 )
    {
      pa = ((unsigned short) lvl.param[i]<<8) + lvl.param[i+1];
      i += 2;
      adr = ((unsigned short) lvl.param[i]<<8) + lvl.param[i+1] - 26;
      i += 2;
      /*out << "param " << i-4 << ": " << pa << ", adr: " << adr << "\n";
	out.flush();*/
      code = LevelParam::Blobs;
      switch( pa )
	{
	case 3:			// Customized Explosions
	  code = LevelParam::Explosions;
	case 4:
	  {
	    nr = lvl.param[adr-2];
	    sl.params.push_back( LevelParam::ParamBlock
				 ( code, vector<unsigned short>() ) );
	    vector<unsigned short>	& pc = sl.params.back().second;
	    pc.push_back( nr );
	    for( j=0; j<nr; j++ )
	      {
		pc.push_back( ((unsigned short) lvl.param[adr]<<8) 
			      + (unsigned short) lvl.param[adr+1] - 1 );
		pc.push_back( 3 ); // size x
		pc.push_back( 3 ); // size y
		if( pa == 3 ) // explosion only
		  {
		    pc.push_back( 1 ); // center pos x
		    pc.push_back( 1 ); // center pos y
		  }
		adr += 2;
		for( k=0; k<9; ++k )
		  {
		    pc.push_back( ((unsigned short) lvl.param[adr]<<8)
				  + (unsigned short) lvl.param[adr+1] );
		    adr += 2;
		  }
	      }
	  }
	  break;
	default:
	  break;
	}
    }
}


const Level* SeriesAtari::level( unsigned x ) const
{
  return( &tb[x] );
}


Level* SeriesAtari::level( unsigned x )
{
  return( &tb[x] );
}


