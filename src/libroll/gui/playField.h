/***************************************************************************
                          playField.h  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2000-2002 by Denis Rivière
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


#ifndef QROLL_GUI_PLAYFIELD_H
#define QROLL_GUI_PLAYFIELD_H


#include <qwidget.h>
#include <set>
#include <map>


class QRMainWin;
class RGameField;

namespace roll
{
  typedef std::map<unsigned, unsigned> RenumList;
  struct QRPlayField_Private;
}


/**	Widget central des fenêtres QRoll
 */
class QRPlayField : public QWidget
{
  Q_OBJECT

public:
  ///	Code touche: joueur, touche jeu
  struct KeyCode
  {
    KeyCode() {}
    KeyCode( unsigned p, unsigned k ) : player( p ), key( k ) {}
    ///	joueur
    unsigned	player;
    ///	code touche jeu
    unsigned	key;
  };
  ///	Codes touches pour tous les joueurs, mode monojoueur
  static std::map<int, KeyCode>	KeyMapMono;
  ///	Codes touches pour tous les joueurs, mode multijoueur
  static std::map<int, KeyCode>	KeyMapMulti;
  ///	Code touche manquant dans le jeu
  static unsigned K_FIRE;

  QRPlayField( const QRMainWin* parentMW, bool usegl, QWidget *parent=0, 
	       const char *name=0 );
  virtual ~QRPlayField();

  ///	Joueur qui a le focus
  unsigned player() const;
  ///	Fixe le joueur qui a le focus
  void setPlayer( unsigned p );
  ///	Players which may use keyboard on this playfield
  const std::set<unsigned> & players() const { return( _players ); }
  ///	Fixe les joueurs qui jouent sur le clavier local
  void setPlayers( const std::set<unsigned> & pl );
  ///	Remplit la table de touches
  static void initKeyCodes();
  bool scalingEnabled() const;
  void enableScaling( bool f );
  float scale() const;
  void setGameSize( unsigned w, unsigned h );
  virtual QSize sizeHint() const;

  bool usesOpenGL() const;
  void setUseOpenGL( bool );

  void paint( const QPoint & p );

  void keyPressedEvent( QKeyEvent* key );
  void keyReleasedEvent( QKeyEvent* key );
  void closeEvent( QCloseEvent* e );
  bool posFromMouse( const QPoint & pos, int & x, int & y );
  QPoint posOfElement( int x, int y );
  void displayFull();
  RGameField* viewport();
  const RGameField* viewport() const;

public slots:
  ///	Reçoit une demande de changement de tableaux
  void stageChanged( unsigned );
  ///	Enlever un joueur
  void removePlayer( unsigned num );
  ///	Renuméroter des joueurs
  void renumPlayers( const roll::RenumList & nums );

protected slots:
  void mousePress( QMouseEvent* e );
  void mouseRelease( QMouseEvent* e );
  void mouseMove( QMouseEvent* e );

signals:
  void keyPressed( const QRPlayField::KeyCode & key, const QRPlayField* win );
  void keyReleased( const QRPlayField::KeyCode & key, const QRPlayField* win );
  void playFieldClosed( QRPlayField* win );
  void paintStart( int x, int y, bool inlevel, QRPlayField* );
  void paintStop( int x, int y, bool inlevel, QRPlayField* );
  void paintMove( int x, int y, bool inlevel, QRPlayField* );
  void displayFullDone( QWidget* );

protected:
  virtual void resizeEvent( QResizeEvent* re );

  ///	Liste des joueurs gérés par cette fenêtre
  std::set<unsigned>		_players;

private:
  roll::QRPlayField_Private	*d;
};


#endif


