/***************************************************************************
                          levelParams.cc  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Riviere
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

#include <roll/gui/levelParams.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qcolordialog.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>

#include <roll/game/vars.h>
#include <roll/struct/series.h>
#include <roll/struct/level.h>
#include <roll/gui/mainWin.h>
#include <roll/struct/simpleLevel.h>
#include <roll/gui/editPalette.h>
#include <math.h>

using namespace roll;
using namespace std;
using namespace Qt;


namespace roll
{
  struct QLevelParams_Private
  {
    QLevelParams_Private()
      : editmode( false ), level( 0 ), sizex( 0 ), sizey( 0 ), diams( 0 ), 
        time( 0 ), perm( 0 ), speed( 0 ), strength( 0 ), genspeed( 0 ),
        valid( 0 ), colpanel( 0 ), exppanel( 0 ), blbpanel( 0 ), expl( 0 ),
        blobs( 0 ), newexpl( 0 ), delexpl( 0 ), newblob( 0 ), delblob( 0 )
    {
      unsigned	i;
      colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = 0;
      for( i=0; i<9; ++i )
      {
        explab[i] = 0;
        blblabb[i] = 0;
        blblabf[i] = 0;
      }
    }

    bool	editmode;
    QSpinBox	*level;
    QSpinBox	*sizex;
    QSpinBox	*sizey;
    QSpinBox	*diams;
    QSpinBox	*time;
    QSpinBox	*perm;
    QSpinBox	*speed;
    QSpinBox	*strength;
    QSpinBox	*genspeed;
    QSpinBox	*blobsize;
    QSpinBox	*bombdelay;

    QPushButton			*valid;
    QGroupBox			*colpanel;
    QGroupBox			*exppanel;
    QGroupBox			*blbpanel;
    QPushButton			*colors[5];
    QComboBox			*expl;
    vector<unsigned short>	explnum;
    QLabel			*explab[9];
    QComboBox			*blobs;
    vector<unsigned short>	blobnum;
    QSpinBox			*blobspesize;
    QLabel			*blblabb[9];
    QLabel			*blblabf[9];
    short			currentExpl;
    short			currentBlob;
    QPushButton                 *newexpl;
    QPushButton                 *delexpl;
    QPushButton                 *newblob;
    QPushButton                 *delblob;
  };

}


QLevelParams::QLevelParams( QWidget* parent, const char* name, 
                            Qt::WindowFlags f )
  : QWidget( parent, f ), d( new QLevelParams_Private )
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( tr( "QRoll level parameters" ) );
  if( name )
    setObjectName( name );

  QHBoxLayout   *mhbl = new QHBoxLayout( this );
  mhbl->setSpacing( 5 );
  mhbl->setMargin( 2 );
  setLayout( mhbl );
  QFrame       *mfr = new QFrame( this );
  mhbl->addWidget( mfr );
  QGridLayout	*mfrl = new QGridLayout( mfr );
  mfr->setLayout( mfrl );
  mfr->setFrameStyle( QFrame::Box | QFrame::Sunken );
  mfrl->setSpacing( 5 );
  mfrl->setMargin( 10 );

  mfrl->addWidget( new QLabel( tr( "Level :" ), mfr ), 0, 0 );
  d->level = new QSpinBox( mfr );
  d->level->setMinimum( 0 );
  d->level->setMaximum( ser->numLevels() - 1 );
  d->level->setButtonSymbols( QSpinBox::PlusMinus );
  mfrl->addWidget( d->level, 0, 1 );

  QWidget	*szxhb = new QWidget( mfr );
  QHBoxLayout   *szxhbl = new QHBoxLayout( szxhb );
  szxhbl->setMargin( 0 );
  mfrl->addWidget( szxhb, 1, 0 );
  szxhb->setLayout( szxhbl );
  szxhbl->setSpacing( 5 );
  szxhbl->addWidget( new QLabel( tr( "Size :" ), szxhb ) );
  d->sizex = new QSpinBox( szxhb );
  d->sizex->setRange( 0, 255 );
  d->sizex->setSingleStep( 16 );
  szxhbl->addWidget( d->sizex );
  QLabel	*xl = new QLabel( "x", szxhb );
  szxhbl->addWidget( xl );
  xl->setFixedWidth( xl->sizeHint().width() );
  d->sizey = new QSpinBox( mfr );
  d->sizey->setRange( 0, 255 );
  d->sizey->setSingleStep( 12 );
  mfrl->addWidget( d->sizey, 1, 1 );

  mfrl->addWidget( new QLabel( tr( "Diams :" ), mfr ), 2, 0 );
  d->diams = new QSpinBox( mfr );
  d->diams->setRange( 0, 65536 );
  mfrl->addWidget( d->diams, 2, 1 );

  mfrl->addWidget( new QLabel( tr( "Time :" ), mfr ), 3, 0 );
  d->time = new QSpinBox( mfr );
  d->time->setRange( 0, 65535 );
  mfrl->addWidget( d->time, 3, 1 );

  mfrl->addWidget( new QLabel( tr( "Wall permeab. :" ), mfr ), 4, 0 );
  d->perm = new QSpinBox( mfr );
  d->perm->setRange( 0, 99 );
  mfrl->addWidget( d->perm, 4, 1 );

  mfrl->addWidget( new QLabel( tr( "Blob speed :" ), mfr ), 5, 0 );
  d->speed = new QSpinBox( mfr );
  d->speed->setRange( 0, 99 );
  mfrl->addWidget( d->speed, 5, 1 );

  mfrl->addWidget( new QLabel( tr( "Strength :" ), mfr ), 6, 0 );
  d->strength = new QSpinBox( mfr );
  d->strength->setRange( 0, 99 );
  mfrl->addWidget( d->strength, 6, 1 );

  mfrl->addWidget( new QLabel( tr( "Gener. speed :" ), mfr ), 7, 0 );
  d->genspeed = new QSpinBox( mfr );
  d->genspeed->setRange( 0, 99 );
  mfrl->addWidget( d->genspeed, 7, 1 );

  mfrl->addWidget( new QLabel( tr( "Blob max. size :" ), mfr ), 8, 0 );
  d->blobsize = new QSpinBox( mfr );
  d->blobsize->setRange( 0, 65535 );
  mfrl->addWidget( d->blobsize, 8, 1 );

  mfrl->addWidget( new QLabel( tr( "Bomb delay :" ), mfr ), 9, 0 );
  d->bombdelay = new QSpinBox( mfr );
  d->bombdelay->setRange( 0, 65535 );
  mfrl->addWidget( d->bombdelay, 9, 1 );

  QWidget	*ap = new QWidget( mfr );
  QHBoxLayout	*apl = new QHBoxLayout( ap );
  mfrl->addWidget( ap, 10, 0 );
  ap->setLayout( apl );
  apl->setSpacing( 5 );
  apl->setMargin( 0 );
  QCheckBox	*colcb = new QCheckBox( tr( "Colors" ), ap );
  apl->addWidget( colcb );
  QCheckBox	*expcb = new QCheckBox( tr( "Explosions" ), ap );
  apl->addWidget( expcb );
  QCheckBox	*blbcb = new QCheckBox( tr( "Blobs" ), mfr );
  mfrl->addWidget( blbcb, 10, 1 );
  QCheckBox	*edit = new QCheckBox( tr( "Edit values..." ), mfr );
  mfrl->addWidget( edit, 11, 0 );
  d->valid = new QPushButton( tr( "Validate" ), mfr );
  mfrl->addWidget( d->valid, 11, 1 );

  // colors panel

  d->colpanel = new QGroupBox( tr( "Colors :" ), this );
  mhbl->addWidget( d->colpanel );
  d->colpanel->hide();
  d->colors[0] = new QPushButton( d->colpanel );
  d->colors[1] = new QPushButton( d->colpanel );
  d->colors[2] = new QPushButton( d->colpanel );
  d->colors[3] = new QPushButton( d->colpanel );
  d->colors[4] = new QPushButton( d->colpanel );
  QSize	sz( 38, 38 );
  d->colors[0]->setFixedSize( sz );
  d->colors[1]->setFixedSize( sz );
  d->colors[2]->setFixedSize( sz );
  d->colors[3]->setFixedSize( sz );
  d->colors[4]->setFixedSize( sz );
  QVBoxLayout *colpl = new QVBoxLayout( d->colpanel );
  colpl->setMargin( 5 );
  colpl->setSpacing( 3 );
  colpl->addWidget( d->colors[0] );
  colpl->addWidget( d->colors[1] );
  colpl->addWidget( d->colors[2] );
  colpl->addWidget( d->colors[3] );
  colpl->addWidget( d->colors[4] );
  colpl->addStretch();

  // explosions panel

  d->exppanel = new QGroupBox( tr( "Explosions :" ), this );
  mhbl->addWidget( d->exppanel );
  QVBoxLayout *exppl = new QVBoxLayout( d->exppanel );
  d->exppanel->hide();
  d->expl = new QComboBox( d->exppanel );
  exppl->addWidget( d->expl );
  d->expl->setFixedSize( 64, 44 );
  d->expl->setIconSize( QSize( 32, 32 ) );
  QPushButton	*enew = new QPushButton( tr( "New" ), d->exppanel );
  d->newexpl = enew;
  exppl->addWidget( enew );
  QPushButton	*ermv = new QPushButton( tr( "Remove" ), d->exppanel );
  d->delexpl = ermv;
  exppl->addWidget( ermv );
  enew->setEnabled( false );
  ermv->setEnabled( false );
  QFrame   *expgd = new QFrame( d->exppanel );
  exppl->addWidget( expgd );
  QGridLayout	*expgdl = new QGridLayout( expgd );
  expgdl->setMargin( 0 );
  expgdl->setSpacing( 0 );
  expgd->setLayout( expgdl );
  expgd->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  unsigned	i;
  for( i=0; i<9; ++i )
  {
    d->explab[i] = new internal::QElemDrawer( i, expgd );
    expgdl->addWidget( d->explab[i], int( i / 3 ), i % 3 );
    d->explab[i]->setFixedSize( 32, 32 );
    connect( d->explab[i], SIGNAL( drawn( int, unsigned short ) ),
              this, SLOT( drawExplosion( int, unsigned short ) ) );
  }
  expgd->setFixedSize( expgd->sizeHint() );
  exppl->addStretch();

  // blobs panel

  d->blbpanel = new QGroupBox( tr( "Blobs :" ), this );
  mhbl->addWidget( d->blbpanel );
  QVBoxLayout *blbl = new QVBoxLayout( d->blbpanel );
  d->blbpanel->hide();
  d->blobs = new QComboBox( d->blbpanel );
  blbl->addWidget( d->blobs );
  d->blobs->setFixedSize( 64, 44 );
  d->blobs->setIconSize( QSize( 32, 32 ) );
  QWidget       *bshb = new QWidget( d->blbpanel );
  QHBoxLayout   *bshbl = new QHBoxLayout( bshb );
  blbl->addWidget( bshb );
  bshbl->addWidget( new QLabel( tr( "Size:" ), bshb ) );
  d->blobspesize = new QSpinBox( bshb );
  bshbl->addWidget( d->blobspesize );
  QPushButton	*bnew = new QPushButton( tr( "New" ), d->blbpanel );
  blbl->addWidget( bnew );
  d->newblob = bnew;
  QPushButton	*brmv = new QPushButton( tr( "Remove" ), d->blbpanel );
  blbl->addWidget( brmv );
  d->delblob = brmv;
  bnew->setEnabled( false );
  brmv->setEnabled( false );
  d->blobspesize->setEnabled( false );
  blbl->addWidget( new QLabel( tr( "Free :" ), d->blbpanel ) );
  QFrame      *blbgd = new QFrame( d->blbpanel );
  QGridLayout *blbgdl = new QGridLayout( blbgd );
  blbgdl->setMargin( 0 );
  blbgdl->setSpacing( 0 );
  blbl->addWidget( blbgd );
  blbgd->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  for( i=0; i<9; ++i )
  {
    d->blblabf[i] = new internal::QElemDrawer( i, blbgd );
    blbgdl->addWidget( d->blblabf[i], int( i / 3 ), i % 3 );
    d->blblabf[i]->setFixedSize( 32, 32 );
    connect( d->blblabf[i], SIGNAL( drawn( int, unsigned short ) ),
              this, SLOT( drawBlobFree( int, unsigned short ) ) );
  }
  blbgd->setFixedSize( blbgd->sizeHint() );
  blbl->addWidget( new QLabel( tr( "Blocked :" ), d->blbpanel ) );
  blbgd = new QFrame( d->blbpanel );
  blbgdl = new QGridLayout( blbgd );
  blbgdl->setMargin( 0 );
  blbgdl->setSpacing( 0 );
  blbl->addWidget( blbgd );
  blbgd->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  for( i=0; i<9; ++i )
  {
    d->blblabb[i] = new internal::QElemDrawer( i, blbgd );
    blbgdl->addWidget( d->blblabb[i], int( i / 3 ), i % 3 );
    d->blblabb[i]->setFixedSize( 32, 32 );
    connect( d->blblabb[i], SIGNAL( drawn( int, unsigned short ) ),
              this, SLOT( drawBlobBlocked( int, unsigned short ) ) );
  }
  blbgd->setFixedSize( blbgd->sizeHint() );

  // signals / slots

  setEditMode( false );
  update( game.tb );

  connect( d->level, SIGNAL( valueChanged( int ) ), this,
            SLOT( changeLevel( int ) ) );
  connect( edit, SIGNAL( toggled( bool ) ), this,
            SLOT( setEditMode( bool ) ) );
  connect( d->valid, SIGNAL( clicked() ), this, SLOT( validate() ) );
  connect( colcb, SIGNAL( toggled( bool ) ), this,
            SLOT( showColors( bool ) ) );
  connect( expcb, SIGNAL( toggled( bool ) ), this,
            SLOT( showExplosions( bool ) ) );
  connect( blbcb, SIGNAL( toggled( bool ) ), this,
            SLOT( showBlobs( bool ) ) );
  connect( d->colors[0], SIGNAL( clicked() ), this, SLOT( editColorRock() ) );
  connect( d->colors[1], SIGNAL( clicked() ), this, SLOT( editColorGrass() ) );
  connect( d->colors[2], SIGNAL( clicked() ), this,
            SLOT( editColorMonster() ) );
  connect( d->colors[3], SIGNAL( clicked() ), this, SLOT( editColorRoll() ) );
  connect( d->colors[4], SIGNAL( clicked() ), this, SLOT( editColorDiam() ) );

  connect( d->expl, SIGNAL( activated( int ) ), this,
            SLOT( explosionSelected( int ) ) );
  connect( enew, SIGNAL( clicked() ), this, SLOT( newExplosion() ) );
  connect( ermv, SIGNAL( clicked() ), this, SLOT( removeExplosion() ) );
  connect( d->blobs, SIGNAL( activated( int ) ), this,
            SLOT( blobSelected( int ) ) );
  connect( bnew, SIGNAL( clicked() ), this, SLOT( newBlob() ) );
  connect( brmv, SIGNAL( clicked() ), this, SLOT( removeBlob() ) );
  connect( d->blobspesize, SIGNAL( valueChanged( int ) ), this,
            SLOT( blobSpecialSizeChanged( int ) ) );
}


QLevelParams::~QLevelParams()
{
  delete d;
}


void QLevelParams::update( unsigned n )
{
  if( n != (unsigned) game.tb )
    cerr << "QLevelParams::update - level hasn't been updated in game !\n";

  d->blobspesize->setRange( 0, 65535 );
  if( d->editmode )
  {
    d->sizex->setRange( 0, 255 );
    d->sizey->setRange( 0, 255 );
    d->diams->setRange( 0, 65535 );
    d->time->setRange( 0, 65535 );
    d->perm->setRange( 0, 99 );
    d->speed->setRange( 0, 99 );
    d->strength->setRange( 0, 99 );
    d->genspeed->setRange( 0, 99 );
    d->blobsize->setRange( 0, 65535 );
    d->bombdelay->setRange( 0, 65535 );
  }
  else
  {
    d->sizex->setRange( game.tbct.sizeX(), game.tbct.sizeX() );
    d->sizey->setRange( game.tbct.sizeY(), game.tbct.sizeY() );
    d->diams->setRange( game.tbct.tab->diams(), game.tbct.tab->diams() );
    d->time->setRange( game.tbct.tab->time(), game.tbct.tab->time() );
    d->perm->setRange( game.tbct.tab->permeability(),
                        game.tbct.tab->permeability() );
    d->speed->setRange( game.tbct.tab->speed(), game.tbct.tab->speed() );
    d->strength->setRange( game.tbct.tab->strength(),
                            game.tbct.tab->strength() );
    d->genspeed->setRange( game.tbct.tab->genSpeed(),
                            game.tbct.tab->genSpeed() );
    d->blobsize->setRange( game.tbct.tab->blobMaxSize(),
                            game.tbct.tab->blobMaxSize() );
    d->bombdelay->setRange( game.tbct.tab->bombDelay(),
                            game.tbct.tab->bombDelay() );
  }

  d->level->setMaximum( ser->numLevels() - 1 );
  d->level->setValue( game.tb );
  d->sizex->setValue( game.tbct.sizeX() );
  d->sizey->setValue( game.tbct.sizeY() );
  d->diams->setValue( game.tbct.tab->diams() );
  d->time->setValue( game.tbct.tab->time() );
  d->perm->setValue( game.tbct.tab->permeability() );
  d->speed->setValue( game.tbct.tab->speed() );
  d->strength->setValue( game.tbct.tab->strength() );
  d->genspeed->setValue( game.tbct.tab->genSpeed() );
  d->blobsize->setValue( game.tbct.tab->blobMaxSize() );
  d->bombdelay->setValue( game.tbct.tab->bombDelay() );

  // colors
  const QPixmap	* const * spr = theQRWin->sprites();
  d->colors[0]->setIcon( *spr[38] );
  d->colors[1]->setIcon( *spr[19] );
  d->colors[2]->setIcon( *spr[0] );
  d->colors[3]->setIcon( *spr[34] );
  d->colors[4]->setIcon( *spr[37] );

  updateExplosions();
  updateBlobs();
}


void QLevelParams::changeLevel( int x )
{
  if( game.running )
    d->level->setValue( game.tb );
  else
    emit levelChanged( x );
}


void QLevelParams::setEditMode( bool x )
{
  d->editmode = x;
  d->valid->setEnabled( x );
  if( x )
    d->valid->show();
  else
    d->valid->hide();
  d->newexpl->setEnabled( x );
  d->delexpl->setEnabled( x );
  d->newblob->setEnabled( x );
  d->delblob->setEnabled( x );
  d->blobspesize->setEnabled( x );
  update( game.tb );
  //resize( minimumSize() ); // doesn't seem to do anything...
}


void QLevelParams::validate()
{
  Level	*lvl = game.tbct.tab;
  game.tbct.setDiams( d->diams->value() );
  game.tbct.setTime( d->time->value() );
  lvl->setPermeability( d->perm->value() );
  lvl->setSpeed( d->speed->value() );
  lvl->setStrength( d->strength->value() );
  lvl->setGenSpeed( d->genspeed->value() );
  game.tbct.setPermeability( d->perm->value() );
  game.tbct.setSpeed( d->speed->value() );
  game.tbct.setStrength( d->strength->value() );
  game.tbct.setGenSpeed( d->genspeed->value() );
  game.tbct.setBlobMaxSize( d->blobsize->value() );
  game.tbct.setBombDelay( d->bombdelay->value() );
  game.tbct.resize( d->sizex->value(), d->sizey->value() );
  game.tbct.setModified( true );

  emit levelChanged( game.tb );
}


void QLevelParams::showColors( bool x )
{
  if( x )
    d->colpanel->show();
  else
    d->colpanel->hide();
  resize( minimumSize() );
}


void QLevelParams::showExplosions( bool x )
{
  if( x )
    d->exppanel->show();
  else
    d->exppanel->hide();
  resize( minimumSize() );
}


void QLevelParams::showBlobs( bool x )
{
  if( x )
    d->blbpanel->show();
  else
    d->blbpanel->hide();
  resize( minimumSize() );
}


void QLevelParams::editColor( int x )
{
  if( !d->editmode )
    return;

  WorkLevel::RGB	colw( game.tbct.color( (WorkLevel::PartColor) x ) );
  QColor	col = QColorDialog::getColor( QColor( 128 + colw.r / 2, 
                                                      128 + colw.g / 2, 
                                                      128 + colw.b / 2 ) );
  if( col.isValid() )
  {
    colw.r = ( (int) col.red() - 128 ) * 2;
    colw.g = ( (int) col.green() - 128 ) * 2;
    colw.b = ( (int) col.blue() - 128 ) * 2;
    game.tbct.setColor( (WorkLevel::PartColor) x, colw );

    game.tbct.setModified( true );
    emit levelChanged( game.tb );
  }
}


void QLevelParams::editColorRock()
{
  editColor( WorkLevel::ROCK );
}


void QLevelParams::editColorGrass()
{
  editColor( WorkLevel::GRASS );
}


void QLevelParams::editColorMonster()
{
  editColor( WorkLevel::MONSTER );
}


void QLevelParams::editColorRoll()
{
  editColor( WorkLevel::ROLL );
}


void QLevelParams::editColorDiam()
{
  editColor( WorkLevel::DIAM );
}


void QLevelParams::updateExplosions()
{
  d->expl->clear();
  d->explnum.clear();
  WorkLevel	& wl = game.tbct;

  vector<SimpleLevel::ParamBlock>	& par = wl.params;
  unsigned		i, j, ne, n = par.size(), ad;
  unsigned short	el;
  const QPixmap	* const * spr = theQRWin->sprites();
  int			nn = -1;

  for( i=0; i<n; ++i )
    if( par[i].first == SimpleLevel::Explosions )
    {
      const vector<unsigned short>	& pb = par[i].second;
      ne = pb[0];
      ad = 1;
      for( j=0; j<ne; ++j )
      {
        el = pb[ ad ];
        d->expl->addItem( *spr[ el ], QString() );
        d->explnum.push_back( el );
        ad += 14;
      }
      if( nn >= 0 )	// several explosion blocks, should not happen!
      { // merge them
        cout << "merging multiple explosions definitions\n";
        vector<unsigned short>	& pbo = par[nn].second;
        pbo[0] += ne;
        for( j=0; j<ne * 14; ++j )
          pbo.push_back( pb[j+1] );
        par.erase( par.begin() + i );
        --i;
      }
      else
        nn = i;
    }
  explosionSelected( 0 );
}


void QLevelParams::explosionSelected( int x )
{
  unsigned	k, l;
  const QPixmap	* const * spr = theQRWin->sprites();

  if( x >= 0 && x < (int) d->explnum.size() )
  {
    unsigned short	el = d->explnum[x];
    unsigned		ad = explo.a[ el ];

    for( k=0; k<3; ++k )
      for( l=0; l<3; ++l )
        d->explab[k*3+l]->setPixmap( *spr[ explo.d[ad][l][k] ] );
    d->currentExpl = el;
  }
  else
  {
    for( k=0; k<3; ++k )
      for( l=0; l<3; ++l )
        d->explab[k*3+l]->setPixmap( *spr[ 255 ] );
    d->currentExpl = -1;
  }
}


void QLevelParams::updateBlobs()
{
  d->blobs->clear();
  d->blobnum.clear();
  WorkLevel	& wl = game.tbct;

  vector<SimpleLevel::ParamBlock>	& par = wl.params;
  unsigned		i, j, ne, n = par.size(), ad;
  unsigned short	el;
  const QPixmap	* const * spr = theQRWin->sprites();
  int			nn = -1;

  for( i=0; i<n; ++i )
    if( par[i].first == SimpleLevel::Blobs )
    {
      const vector<unsigned short>	& pb = par[i].second;
      ne = pb[0];
      ad = 1;
      if( pb.size() != ne * 24 + 1 )
        cout << "mismatch: pb: " << pb.size() << ", ne : " << ne << endl;
      for( j=0; j<ne; ++j )
      {
        el = pb[ ad ];
        d->blobs->addItem( *spr[ el ], QString() );
        d->blobnum.push_back( el );
        ad += 24;
      }
      if( nn >= 0 )	// several blob blocks, should not happen!
      { // merge them
        cout << "merging multiple blobs definitions\n";
        vector<unsigned short>	& pbo = par[nn].second;
        pbo[0] += ne;
        for( j=0; j<ne * 24; ++j )
          pbo.push_back( pb[j+1] );
        par.erase( par.begin() + i );
        --i;
      }
      else
        nn = i;
    }
  blobSelected( 0 );
}


void QLevelParams::blobSelected( int x )
{
  unsigned	k, l;
  const QPixmap	* const * spr = theQRWin->sprites();

  if( x >= 0 && x < (int) d->blobnum.size() )
  {
    unsigned short	el = d->blobnum[x];
    unsigned		ad = explo.a[ el ];

    d->blobspesize->setValue( bebet[ad].tmax );
    for( k=0; k<3; ++k )
      for( l=0; l<3; ++l )
      {
        d->blblabf[k*3+l]->
          setPixmap( *spr[ bebet[ad].d1[l][k] ] );
        d->blblabb[k*3+l]->
          setPixmap( *spr[ bebet[ad].d2[l][k] ] );
      }
    d->currentBlob = el;
  }
  else
  {
    d->blobspesize->setValue( 0 );
    for( k=0; k<3; ++k )
      for( l=0; l<3; ++l )
      {
        d->blblabf[k*3+l]->setPixmap( *spr[ 255 ] );
        d->blblabb[k*3+l]->setPixmap( *spr[ 255 ] );
      }
    d->currentBlob = -1;
  }
}


void QLevelParams::drawExplosion( int x, unsigned short elem )
{
  if( !d->editmode || d->currentExpl < 0 )
    return;
  const QPixmap	* const * spr = theQRWin->sprites();
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Explosions )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentExpl )
        {
          ad += 5; // skip element num and sizes
          if( pb[ ad + x ] != elem )
          {
            game.tbct.setModified( true );
            pb[ ad + x ] = elem;
            d->explab[x]->setPixmap( *spr[ elem ] );
            unsigned	rc = explo.a[ d->currentExpl ];
            explo.d[rc][x%3][x/3] = elem;
          }
          break;
        }
        ad += 14;
      }
      break;
    }
}


void QLevelParams::drawBlobBlocked( int x, unsigned short elem )
{
  // cout << "drawBlobBlocked " << x << ", " << elem << endl;
  if( !d->editmode || d->currentBlob < 0 )
    return;
  const QPixmap	* const * spr = theQRWin->sprites();
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Blobs )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentBlob )
        {
          ad += 15;
          if( pb[ ad + x ] != elem )
          {
            game.tbct.setModified( true );
            pb[ ad + x ] = elem;
            d->blblabb[x]->setPixmap( *spr[ elem ] );
            unsigned	rc = explo.a[ d->currentBlob ];
            bebet[rc].d1[x%3][x/3] = elem;
          }
          break;
        }
        ad += 24;
      }
      break;
    }
}


void QLevelParams::drawBlobFree( int x, unsigned short elem )
{
  // cout << "drawBlobFree " << x << ", " << elem << endl;
  if( !d->editmode || d->currentBlob < 0 )
    return;
  const QPixmap	* const * spr = theQRWin->sprites();
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Blobs )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentBlob )
        {
          ad += 4;
          if( pb[ ad + x ] != elem )
          {
            game.tbct.setModified( true );
            pb[ ad + x ] = elem;
            d->blblabf[x]->setPixmap( *spr[ elem ] );
            unsigned	rc = explo.a[ d->currentBlob ];
            bebet[rc].d2[x%3][x/3] = elem;
          }
          break;
        }
        ad += 24;
      }
      break;
    }
}


void QLevelParams::newExplosion()
{
  if( !d->editmode )
    return;
  set<unsigned short>		elist;
  set<unsigned>			addresses;
  set<unsigned>::iterator	notused = addresses.end();
  unsigned			i, n;

  for( i=0; i<512; ++i )
  {
    if( expli.a[i] >= 0 && addresses.find( expli.a[i] ) == notused )
    {
      GElem	*elem = elFactory.createElem( i );
      if( elem && elem->isExplosive() )
      {
        elist.insert( i );
        addresses.insert( expli.a[i] );
      }
      delete elem;
    }
  }
  for( i=0, n=d->explnum.size(); i<n; ++i )
    elist.erase( d->explnum[i] );

  internal::ElementSelector	esel( elist, 0, tr( "Edit explosion" ), true );
  if( esel.exec() )
  {
    unsigned short	elem = (unsigned short) esel.button;
    vector<LevelParam::ParamBlock>	& par = game.tbct.params;
    unsigned				p, n = par.size(), i, j, ad;
    ad = expli.a[elem];
    for( p=0; p<n; ++p )
      if( par[p].first == LevelParam::Explosions )
        break;
    if( p == par.size() )
    {
      par.push_back( LevelParam::ParamBlock( LevelParam::Explosions,
                                              vector<unsigned short>() ) );
      par[p].second.push_back( 0 );
    }

    vector<unsigned short>	&pb = par[p].second;
    ++pb[0];
    pb.push_back( elem );
    pb.push_back( 3 ); // size x
    pb.push_back( 3 ); // size y
    pb.push_back( 1 ); // center x
    pb.push_back( 1 ); // center y
    for( j=0; j<3; ++j )
      for( i=0; i<3; ++i )
        pb.push_back( expli.d[ad][i][j] );

    game.tbct.setModified( true );
    emit levelChanged( game.tb );
    //d->expl->setCurrentItem( p );
    //explosionSelected( p );
  }
}


void QLevelParams::removeExplosion()
{
  if( !d->editmode || d->currentExpl < 0 )
    return;
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Explosions )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentExpl )
        {
          if( ne == 1 )
            par.erase( par.begin() + p );
          else
          {
            pb.erase( pb.begin() + ad, pb.begin() + ad + 14 );
            --pb[0];
          }
          game.tbct.setModified( true );
          emit levelChanged( game.tb );
          break;
        }
        ad += 14;
      }
      break;
    }
}


void QLevelParams::newBlob()
{
  if( !d->editmode )
    return;
  set<unsigned short>		elist;
  set<unsigned>			addresses;
  set<unsigned>::iterator	notused = addresses.end();
  unsigned			i, n;

  for( i=0; i<512; ++i )
    if( expli.a[i] && addresses.find( expli.a[i] ) == notused )
    {
      GElem	*elem = elFactory.createElem( i );
      if( elem && elem->isBlob() )
      {
        elist.insert( i );
        addresses.insert( expli.a[i] );
      }
      delete elem;
    }
  for( i=0, n=d->blobnum.size(); i<n; ++i )
    elist.erase( d->blobnum[i] );

  internal::ElementSelector	esel( elist, 0, tr( "Edit Blob" ), true );
  if( esel.exec() )
  {
    unsigned short	elem = (unsigned short) esel.button;
    vector<LevelParam::ParamBlock>	& par = game.tbct.params;
    unsigned				p, n = par.size(), i, j, ad;
    ad = expli.a[elem];
    for( p=0; p<n; ++p )
      if( par[p].first == LevelParam::Blobs )
        break;
    if( p == par.size() )
    {
      par.push_back( LevelParam::ParamBlock( LevelParam::Blobs,
                                              vector<unsigned short>() ) );
      par[p].second.push_back( 0 );
    }

    vector<unsigned short>	&pb = par[p].second;
    ++pb[0];
    pb.push_back( elem );
    pb.push_back( 256 );	// size
    pb.push_back( 3 );        // pattern size x
    pb.push_back( 3 );        // pattern size y
    for( j=0; j<3; ++j )
      for( i=0; i<3; ++i )
        pb.push_back( bebi[ad].d1[i][j] );
    pb.push_back( 3 );        // pattern size x
    pb.push_back( 3 );        // pattern size y
    for( j=0; j<3; ++j )
      for( i=0; i<3; ++i )
        pb.push_back( bebi[ad].d2[i][j] );

    game.tbct.setModified( true );
    emit levelChanged( game.tb );
    //d->blobs->setCurrentItem( p );
    //blobSelected( p );
  }
}


void QLevelParams::removeBlob()
{
  if( !d->editmode || d->currentBlob < 0 )
    return;
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Blobs )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentBlob )
        {
          if( ne == 1 )
            par.erase( par.begin() + p );
          else
          {
            pb.erase( pb.begin() + ad, pb.begin() + ad + 24 );
            --pb[0];
          }
          game.tbct.setModified( true );
          emit levelChanged( game.tb );
          break;
        }
        ad += 24;
      }
      break;
    }
}


void QLevelParams::blobSpecialSizeChanged( int x )
{
  if( !d->editmode || d->currentBlob < 0 )
    return;
  vector<LevelParam::ParamBlock>	& par = game.tbct.params;
  unsigned				p, n = par.size(), ne, i, ad;
  for( p=0; p<n; ++p )
    if( par[p].first == LevelParam::Blobs )
    {
      vector<unsigned short>	&pb = par[p].second;
      ne = pb[0];
      ad = 1;
      for( i=0; i<ne; ++i )
      {
        if( pb[ad] == d->currentBlob )
        {
          if( pb[ ad + 1 ] != x )
          {
            pb[ad+1] = x;
            game.tbct.setModified( true );
            unsigned	rc = explo.a[ d->currentBlob ];
            bebet[rc].tmax = x;
          }
          break;
        }
        ad += 24;
      }
      break;
    }
}


internal::QElemDrawer::QElemDrawer( int x, QWidget * parent, 
                                    Qt::WindowFlags f )
  : QLabel( parent, f ), _x( x )
{
}


internal::QElemDrawer::~QElemDrawer()
{
}


void internal::QElemDrawer::mousePressEvent( QMouseEvent* ev )
{
  drawElement( ev );
}


void internal::QElemDrawer::mouseMoveEvent( QMouseEvent* ev )
{
  drawElement( ev );
}


void internal::QElemDrawer::drawElement( QMouseEvent* )
{
  QREditPalette	*ep = theQRWin->editPalette();
  if( !ep )
    return;
#ifndef ANDROID
  emit drawn( _x, ep->selectedSprite() );
#endif
}


internal::ElementSelector::ElementSelector( const set<unsigned short> 
					    & choice, QWidget* parent, 
					    const QString & name,
                                            bool modal, Qt::WindowFlags f )
  : QDialog( parent, f ), button( -1 )
{
  setWindowTitle( name );
  setModal( modal );
  QVBoxLayout	*lay = new QVBoxLayout( this );
  lay->setMargin( 10 );
  lay->setSpacing( 10 );
  QGroupBox *grd = new QGroupBox( name, this );
  QGridLayout *gl = new QGridLayout( grd );
  QButtonGroup	*grdb = new QButtonGroup(grd );
  grdb->setExclusive( true );
  const QPixmap	* const * spr = theQRWin->sprites();
  QPushButton	*btn;
  int i=0, n = (int) sqrt( (double) choice.size() );

  lay->addWidget( grd );
  set<unsigned short>::const_iterator	ic, ec = choice.end();
  for( ic=choice.begin(); ic!=ec; ++ic, ++i )
  {
    _choice.push_back( *ic );
    btn = new QPushButton( grd );
    gl->addWidget( btn, int( i / n ), i % n, Qt::AlignHCenter );
    grdb->addButton( btn, i );
    btn->setIcon( *spr[ *ic ] );
    btn->setFixedSize( btn->sizeHint() );
  }

  QPushButton	*can = new QPushButton( QLevelParams::tr( "Cancel" ), this );
  can->setFixedSize( can->sizeHint() );
  lay->addWidget( can );
  connect( can, SIGNAL( clicked() ), this, SLOT( reject() ) );
  connect( grdb, SIGNAL( buttonClicked( int ) ), this,
           SLOT( selectElement( int ) ) );
}


internal::ElementSelector::~ElementSelector()
{
}


void internal::ElementSelector::selectElement( int x )
{
  cout << "selectElement " << x << endl;
  button = _choice[x];
  accept();
}


