/***************************************************************************
                          levelParams.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001 by Denis Rivi�e
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


#ifndef QROLL_GUI_LEVELPARAMS_H
#define QROLL_GUI_LEVELPARAMS_H

#include <qlabel.h>
#include <qdialog.h>
#include <set>
#include <vector>

namespace roll
{
  struct QLevelParams_Private;
}


class QLevelParams : public QWidget
{
  Q_OBJECT

public:
  QLevelParams( QWidget* parent = 0, const char* name = 0, 
                Qt::WFlags f = Qt::WDestructiveClose );
  virtual ~QLevelParams();

public slots:
  void update( unsigned );

signals:
  // void levelParamsDestroyed();
  void levelChanged( unsigned );

private:
  void updateExplosions();
  void updateBlobs();

private slots:
  void changeLevel( int );
  void setEditMode( bool );
  void validate();
  void showColors( bool );
  void showExplosions( bool );
  void showBlobs( bool );
  void editColorRock();
  void editColorGrass();
  void editColorMonster();
  void editColorRoll();
  void editColorDiam();
  void editColor( int x );
  void explosionSelected( int );
  void blobSelected( int );
  void drawExplosion( int, unsigned short );
  void drawBlobBlocked( int, unsigned short );
  void drawBlobFree( int, unsigned short );
  void newExplosion();
  void removeExplosion();
  void newBlob();
  void removeBlob();
  void blobSpecialSizeChanged( int );

private:
  roll::QLevelParams_Private	*d;
};


namespace roll
{
  namespace internal
  {
    class QElemDrawer : public QLabel
    {
      Q_OBJECT

    public:
      QElemDrawer( int x, QWidget * parent, const char * name=0, 
                   Qt::WFlags f=0 );
      virtual ~QElemDrawer();

    signals:
      void drawn( int x, unsigned short elem );

    protected:
      virtual void mousePressEvent( QMouseEvent* ev );
      virtual void mouseMoveEvent( QMouseEvent* ev );
      virtual void drawElement( QMouseEvent* ev );

    private:
      int	_x;
    };

    class ElementSelector : public QDialog
    {
      Q_OBJECT

    public:
      ElementSelector( const std::set<unsigned short> &, QWidget * parent=0, 
                       const char * name=0, bool modal=FALSE, Qt::WFlags f=0 );
      virtual ~ElementSelector();

      int button;

    public slots:
      void selectElement( int );

    private:
      std::vector<unsigned short>	_choice;
    };

  }
}

#endif


