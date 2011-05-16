/***************************************************************************
                          mainWin.h  -  description
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


#ifndef QROLL_GUI_MAINWIN_H
#define QROLL_GUI_MAINWIN_H


#include <qmainwindow.h>
#include <qmenu.h>
#include <qtoolbutton.h>
#include <roll/gui/playField.h>
#include <roll/struct/workLevel.h>
#include <roll/game/gElem.h>

class QToolBar;
class QRMainWin;
class QREditPalette;

namespace roll
{
  class Painter;
  typedef std::map<unsigned, unsigned> RenumList;

  ///	Global pointer to the main window
  extern QRMainWin	*theQRWin;
}

/* Uncomment the following line if you use windows 9x and have problems with 
   many pixmaps. This bug is documened in Qt documentation, in the QPixmap
   class doc. 
   If you define the WIN32_AVOID_PIXMAPS macro, we use QImage for the 
   orgiginal sprites (not recolored), this will slow down the edition palette 
   and the series arranger.
*/
// #define WIN32_AVOID_PIXMAPS


/**	Fenetre principale QRoll
 */
class QRMainWin : public QMainWindow
{
  Q_OBJECT

public:
  QRMainWin( QWidget *parent=0, const char *name="Rock'N'Roll" );
  ~QRMainWin();

  void load( const char* filename );
  void save( const char* filename );
  const QPixmap * const * sprites() const { return( _sprite ); }
  QPixmap sprite( unsigned short num ) const { return( *_sprite[ num ] ); }
  QPixmap originalSprite( unsigned short num ) const;

  bool usesOpenGL() const;
  void setUseOpenGL( bool x );

  void setTempo( unsigned x );
  unsigned tempo() const { return( _tempo ); }

  bool editMode() const;
  void setEditMode( bool );
  QREditPalette *editPalette();
  /// stores current working level in series if modified
  void storeLevel();
  /// emits stageChanged() signal
  void changeLevel( unsigned level );
  const std::vector<roll::Painter*> & painters() const;

  ///	Converts a RGB to color group index
  static roll::WorkLevel::PartColor gameColor( QRgb rgb );
  static void setLanguage( const std::string & l );
  static std::string language();

public slots:
  ///	Full step routine: draws the level
  void stepFull();
  void load();
  void mergeSeries();
  void save();
  void saveAs();
  void start();
  void stop();
  void pause();
  void pauseON();
  void pauseOFF();
  void nextStage();
  void prevStage();
  void netServer();
  void netClient();
  void netClose();
  void clientHostFound();
  void localClientConnected();
  void localServerConnected();
  void remoteClientConnected( int );
  void remoteClientDisconnected( int );
  void remoteServerDisconnected();
  void netError( int err );
  void receiveKeyPressed( const QRPlayField::KeyCode & key, 
			  const QRPlayField* win );
  void receiveKeyReleased( const QRPlayField::KeyCode & key, 
			   const QRPlayField* win );
  ///	Fin du temps de jeu
  void timeOut();
  ///	timer callback, called twice per turn (for full / half screen updates)
  void timeStep();
  void gameStarts( unsigned level, unsigned randSeed, 
		   const std::string & series );
  void gamePauses( bool state );
  /// signal received by client games from the server
  void turnEnds();
  void addPlayer();
  void removePlayer();
  void closePlayField( QRPlayField * win );
  ///	Rï¿½git ï¿½un ordre de dï¿½enregistrement d'un joueur
  void playerRemoved( unsigned num );
  ///	Callback du timer temps de jeu
  void clockTick();
  ///	Callback rï¿½eau
  void playerAdded( unsigned );
  ///	Callback rï¿½eau
  void playersRenumed( const roll::RenumList & play );
  void about();
  void soundOnOff();
  void allowScale();
  void standardSize();
  void configure();
  void configClosed();
  void editLevel();
  void editPaletteClosed();
  void editModeChanged( unsigned );
  void levelParams();
  void levelParamsClosed();
  void turnEndEmitted();
  void insertLevel();
  void deleteLevel();
  void arrangeSeries();
  void seriesArrangerClosed();
  void updateSeries();
  void paintStart( int x, int y, bool inlevel, QRPlayField* );
  void paintMove( int x, int y, bool inlevel, QRPlayField* );
  void paintStop( int x, int y, bool inlevel, QRPlayField* );

signals:
  ///	game stops
  void gameStopped();
  ///	order to update the level
  void stageChanged( unsigned );
  ///	Level display
  void displayFull();
  ///	half-scrolling display
  void displayHalf();
  ///	Emitted after each 'step' to say that the score may have changed
  void scoreChanged();
  void stopped();
  ///	End of network transmission (or error)
  void connectionClosed( unsigned num );
  ///	Tells windows to remove a player
  void removePlayer( unsigned num );
  ///	Tells windows to renum some players
  void renumPlayers( const roll::RenumList & nums );
  ///	Game time, emitted every second
  void timeTick();
  void seriesChanged();

protected slots:
  void changeLevelMyself( unsigned n );

protected:
  void keyPressEvent( QKeyEvent* key );
  void keyReleaseEvent( QKeyEvent* key );

  ///	Initialise les sprites (graphique)
  void initPixmaps();
  ///	Converts image -> sprite data, grey level + color type
  void convertSprite( const QImage & img, unsigned x, unsigned y, unsigned w, 
		      unsigned h, unsigned char* gbuf, unsigned char* cbuf );
  ///	Convertit les couleurs d'un sprite
  void fillImage( QImage & img, unsigned num );
  ///	Conversion des couleurs des sprites
  void genPixmaps();
  ///	things to do at half-scrolling time
  void stepHalf();

  bool		_paused;

private:
  struct Private;

  QRPlayField			*_playField;
  std::set<QRPlayField *>	_otherWins;

  QMenu                         *_file;
  QMenu                         *_game;
  QMenu                         *_networkMen;
  QMenu                 	*_soundM;
  QMenu 			*_viewM;
  QToolBar			*_tools;

  ///	Niveaux de gris des sprites
  unsigned char			*_greySprite[512];
  ///	Champs de couleurs des sprites
  unsigned char			*_colorSprite[512];
  ///	Sprites
  QPixmap			*_sprite[512];
  ///	Uncolored sprites (original colors)
#if defined( _WIN32 ) && defined( WIN32_AVOID_PIXMAPS )
  QImage			*_osprite[512];
#else
  QPixmap			*_osprite[512];
#endif

  ///	Timer cadence du jeu
  QTimer			*_timer;
  ///	Game execution frequency (ms/step)
  unsigned			_tempo;
  ///	Flag 1/2 timer pour le 1/2 scroll
  bool				_tick;
  ///	Timer temps de jeu
  QTimer			*_clockTimer;
  Private               	*d;
};


#endif



