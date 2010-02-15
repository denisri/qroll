/***************************************************************************
                          clientDialog.h  -  description
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


#ifndef ROLL_GUI_CLIENTDIALOG_H
#define ROLL_GUI_CLIENTDIALOG_H


#include <qdialog.h>
#include <string>

class QLineEdit;


/**	Fiche de dialogue pour choisir la destination de la connexion réseau
 */
class RRClientDialog : public QDialog
{
  Q_OBJECT

public:
  enum ConnectMode
    {
      LOCAL,
      DISTANT
    };

  RRClientDialog( QWidget* parent, const char* name = 0 );
  virtual ~RRClientDialog();

  std::string address() const;
  ConnectMode mode() const { return( _mode ); }

public slots:
  void modeClick( int id );

signals:

protected:
  std::string	_localAddress;
  QLineEdit	*_edit;
  ConnectMode	_mode;

private:
};


#endif

