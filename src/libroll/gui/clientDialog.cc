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
#if QT_VERSION >= 0x040000
#include <Q3GroupBox>
#include <Q3VButtonGroup>
#include <q3grid.h>
#include <q3hbox.h>
#include <q3vbox.h>
typedef Q3VButtonGroup QVButtonGroup;
typedef Q3Grid QGrid;
typedef Q3HBox QHBox;
typedef Q3VBox QVBox;
#else
#include <qgroupbox.h>
#include <qvbuttongroup.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qvbox.h>
typedef QGroupBox Q3GroupBox;
#endif
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock.h>
#else	// _WIN32
#include <unistd.h>
#endif	// _WIN32

using namespace std;


RRClientDialog::RRClientDialog( QWidget* parent, const char* name ) 
  : QDialog( parent, name, true ), _mode( LOCAL )
{
  setCaption( tr( "Network Client : connection choice" ) );

  QVBoxLayout	*lay2 = new QVBoxLayout( this, 10 );

  Q3GroupBox	*hg = new Q3GroupBox( 1, Qt::Vertical, tr( "Server address :" ), this );
  QVButtonGroup	*bts = new QVButtonGroup( hg );
#if QT_VERSION >= 0x040000
  bts->setFlat( true );
#else
  bts->setFrameStyle( QFrame::NoFrame );
#endif
  QVBox		*vals = new QVBox( hg );
  QRadioButton	*locb = new QRadioButton( tr( "Local :" ), bts );
  locb->setChecked( true );

  char	host[50];
  int	res = gethostname( host, 50 );
  if( res < 0 )
    sprintf( host, "?? unknown ??" );
  _localAddress = host;
  new QLabel( host, vals );

  new QRadioButton( tr( "Distant :" ), bts );

  _edit = new QLineEdit( vals );
  _edit->setEnabled( false );

  QHBox		*resFr = new QHBox( this );
  resFr->setSpacing (10 );
  QPushButton	*okBtn = new QPushButton( tr( "Connect" ), resFr, "ok" );
  QPushButton	*cancelBtn = new QPushButton( tr( "Cancel" ), resFr, 
					      "cancel" );
  okBtn->setFixedSize( okBtn->sizeHint() );
  okBtn->setDefault( true );
  cancelBtn->setFixedSize( cancelBtn->sizeHint() );

  lay2->addWidget( hg );
  lay2->addWidget( resFr );

  connect( okBtn, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
  connect( bts, SIGNAL( clicked( int ) ), this, SLOT( modeClick( int ) ) );
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
      return( string( _edit->text().ascii() ) );
    }
}


