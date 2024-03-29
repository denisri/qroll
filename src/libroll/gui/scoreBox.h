/***************************************************************************
                          scoreBox.h  -  description
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


#ifndef QROLL_GUI_SCOREBOX_H
#define QROLL_GUI_SCOREBOX_H


#include <qlabel.h>
#include <qlcdnumber.h>
class QPushButton;
class QButtonGroup;


/**	Barre de scores, timing
 */
class QRScoreBox : public QWidget
{
  Q_OBJECT

public:
  QRScoreBox( const QPixmap * const * sprites, QWidget *parent=0, 
              const char *name="scoreBox" );
  ~QRScoreBox();

  unsigned time() const;
  unsigned score() const;
  unsigned lives() const;
  unsigned diams() const;
  void changeStage();
  void setTime( unsigned t );
  void setScore( unsigned score );
  void setLives( unsigned lives );
  void setDiams( unsigned diams );
  void setPlayer( unsigned num );
  void setBombs( unsigned bombid, unsigned num );
  void setKey( int key );

public slots:
  void oneLessSecond();
  ///	Demande de réaffichage des scores
  void changeScore();
  void bombSelected( int );

signals:

protected:
  QLabel	*_timeLabel;
  QLabel	*_scoreLabel;
  QLabel	*_livesLabel;
  QLabel	*_diamsLabel;
  QLCDNumber	*_time;
  QLCDNumber	*_score;
  QLCDNumber	*_lives;
  QLCDNumber	*_diams;
  QLCDNumber	*_bombs[8];
  QFrame	*_key;
  QPushButton	*_bombsT[8];
  QButtonGroup  *_bombgroup;
  const QPixmap * const * _sprites;
  int           _bombToLaunch;
  int           _keyid;
  unsigned	_player;

private:
};


//	inline

inline unsigned QRScoreBox::time() const
{
  return( (unsigned) _time->intValue() );
}


inline unsigned QRScoreBox::score() const
{
  return( (unsigned) _score->intValue() );
}


inline unsigned QRScoreBox::lives() const
{
  return( (unsigned) _lives->intValue() );
}


inline unsigned QRScoreBox::diams() const
{
  return( (unsigned) _diams->intValue() );
}



#endif


