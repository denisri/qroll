/***************************************************************************
                          clientDialog.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis RiviÃ¯Â¿Â½e
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


#include <roll/gui/clientDialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock.h>
#else	// _WIN32
#include <unistd.h>
#endif	// _WIN32

using namespace std;


RRClientDialog::RRClientDialog( QWidget* parent, const char* name ) 
  : QDialog( parent ), _mode( LOCAL )
{
  if( name )
    setObjectName( name );
  setWindowTitle( tr( "Network Client : connection choice" ) );
  setModal( true );

  QVBoxLayout *lay2 = new QVBoxLayout( this );
  lay2->setContentsMargins( 10, 10, 10, 10 );
  setLayout( lay2 );

  QGroupBox	*hg = new QGroupBox( tr( "Server address :" ), this );
  QGridLayout	*hgvl = new QGridLayout( hg );
  hg->setLayout( hgvl );
  QButtonGroup	*bts = new QButtonGroup( hg );
  QRadioButton	*locb = new QRadioButton( tr( "Local :" ), hg );
  bts->addButton( locb );
  hgvl->addWidget( locb, 0, 0 );
  locb->setChecked( true );

  char	host[50];
  int	res = gethostname( host, 50 );
  if( res < 0 )
    sprintf( host, "?? unknown ??" );
  _localAddress = host;
  hgvl->addWidget( new QLabel( host, hg ), 0, 1 );

  QRadioButton *db = new QRadioButton( tr( "Distant :" ), hg );
  bts->addButton( db );
  hgvl->addWidget( db, 1, 0 );

  _edit = new QLineEdit( hg );
  _edit->setEnabled( false );
  hgvl->addWidget( _edit, 1, 1 );

  lay2->addWidget( hg );
  QWidget *dum = new QWidget( this );
  lay2->addWidget( dum );
  QHBoxLayout	*resFr = new QHBoxLayout( dum );
  dum->setLayout( resFr );
  resFr->setSpacing( 10 );
  QPushButton	*okBtn = new QPushButton( tr( "Connect" ), dum );
  okBtn->setObjectName( "ok" );
  resFr->addWidget( okBtn );
  QPushButton	*cancelBtn = new QPushButton( tr( "Cancel" ), dum );
  cancelBtn->setObjectName( "cancel" );
  resFr->addWidget( cancelBtn );
  okBtn->setFixedSize( okBtn->sizeHint() );
  okBtn->setDefault( true );
  cancelBtn->setFixedSize( cancelBtn->sizeHint() );

  connect( okBtn, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
#if QT_VERSION >= 0x050f00
  connect( bts, SIGNAL( idClicked( int ) ), this, SLOT( modeClick( int ) ) );
#else
  connect( bts, SIGNAL( buttonClicked(int) ), this, SLOT( modeClick( int ) ) );
#endif
}


RRClientDialog::~RRClientDialog()
{
}


void RRClientDialog::modeClick( int id )
{
  if( id == 0 )
    {
      _mode = LOCAL;
      _edit->setEnabled( false );
    }
  else
    {
      _mode = DISTANT;
      _edit->setEnabled( true );
    }
}


string RRClientDialog::address() const
{
  switch( _mode )
    {
    case LOCAL:
      return( _localAddress );
    case DISTANT:
    default:
      return( string( _edit->text().toUtf8().data() ) );
    }
}


