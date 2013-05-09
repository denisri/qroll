/***************************************************************************
                          editPalatte.h  -  description
                             -------------------
    begin                : 2001
    copyright            : (C) 2001-2004 by Denis Riviï¿½e
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


#ifndef QROLL_GUI_EDITPALETTE_H
#define QROLL_GUI_EDITPALETTE_H


#include <qmainwindow.h>
#include <qtoolbutton.h>


/**	Sprites palette for drawing tools of the levels editor.
	This window must be visible as long as the editor mode is active. If 
	closed, R&R returns to game mode
 */
class QREditPalette : public QMainWindow
{
  Q_OBJECT

public:
  QREditPalette( QWidget* parent = 0, const char* name = 0, 
                 Qt::WindowFlags f = 0 );
  virtual ~QREditPalette();

  unsigned short selectedSprite() const;
  unsigned drawingMode() const;

signals:
  void palDestroyed();
  void modeChanged( unsigned );

public slots:
  void select( int );
  void quickSelect( int );
  void quickSelect0();
  void quickSelect1();
  void setDrawingMode( unsigned );
  void scale1();
  void scale2();
  void scale4();
  void scale8();
  void scale( float x );

private:
  struct Private;

  void fillPalette();

  Private	*d;
};


class EditorButton : public QToolButton
{
  Q_OBJECT

public:
  EditorButton( unsigned num, const QPixmap & pm, const QString & textLabel, 
		const QString & grouptext, QObject * receiver, 
		const char * slot, QToolBar * parent, const char * name = 0 );
  virtual ~EditorButton();

  int number() const;

signals:
  void clicked( unsigned );
  void toggled( bool, unsigned );

private slots:
  void emitClicked();
  void emitToggled( bool );

private:
  unsigned _num;
};


#endif


