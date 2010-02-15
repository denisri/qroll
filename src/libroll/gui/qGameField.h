/***************************************************************************
                          qGameField.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2001 by Denis Rivi�e
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

#ifndef ROLL_GUI_QGAMEFIELD_H
#define ROLL_GUI_QGAMEFIELD_H

#include <roll/gui/gameField.h>
#include <qwidget.h>
#include <qpixmap.h>


class QRGameField : public QWidget, public RGameField
{
  Q_OBJECT

public:
  QRGameField( const QPixmap * const * sprites, QWidget *parent, 
	       const char *name = 0 );
  virtual ~QRGameField();

  virtual int width() const { return( QWidget::width() ); }
  virtual int height() const { return( QWidget::height() ); }
  virtual void displayFull();

public slots:
  ///	Draws the level
  void displayFullSlot();
  ///	Draws the half-scroll
  void displayHalfSlot();
  void resetSlot();

signals:
  void mousePress( QMouseEvent* );
  void mouseRelease( QMouseEvent* );
  void mouseMove( QMouseEvent* );
  void displayFullDone( QWidget* );

protected:
  virtual void resizeEvent( QResizeEvent * );
  virtual void keyPressedEvent( QKeyEvent* key );
  virtual void keyReleasedEvent( QKeyEvent* key );
  virtual void mousePressEvent( QMouseEvent* e );
  virtual void mouseReleaseEvent( QMouseEvent* e );
  virtual void mouseMoveEvent( QMouseEvent* e );
  virtual void paintEvent( QPaintEvent* e );

  ///	Prepares the screen or Pixmal in which the level has to be drawn
  virtual void setupScreen( unsigned w, unsigned h );
  ///	Draws a sprite at position (i,j) in the drawing zone
  virtual void copySprite( unsigned spr, int posx, int posy );
  ///	Actually displays the drawing zone
  virtual void updateScreen( bool eraseBorder, int w = 0, int h = 0 );
  virtual void updateScreenQt();

private:
  QPixmap		*_pscr;
  ///	Pointer to the graphics
  const QPixmap		* const *_sprite;
  bool                  _eraseborder;
  int                   _pwidth;
  int                   _pheight;

};


#endif


