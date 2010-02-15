/***************************************************************************
                          gameConfig.h  -  description
                             -------------------
    begin                : 2007
    copyright            : (C) 2001-2007 by Denis Riviere
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

#ifndef ROLL_GUI_GAMECONFIG
#define ROLL_GUI_GAMECONFIG

#include <string>

namespace roll
{

  class GameConfig
  {
  public:
    GameConfig();
    ~GameConfig();

    void load();
    void save();

    static GameConfig & config();
    static std::string homedir();
    static std::string configDir();
    static std::string configFilename();

    void setAllRollsMustExit( bool );
    bool allRollsMustExit() const;
    void setUseOpenGL( bool );
    bool usesOpenGL() const;
    void setGLMode( int );
    int glMode() const;
    void setUseCustomTiming( bool );
    bool usesCustomTiming() const;
    void setTiming( int );
    int timing() const;
    bool allowZoom() const;
    void setAllowZoom( bool );
    bool fullScreen() const;
    void setFullScreen( bool );
    bool mute() const;
    void setMute( bool );

  private:
    struct Private;
    Private *d;
  };

}

#endif

