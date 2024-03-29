
set( _sources 
  struct/workLevel.cc
  struct/series.cc
  struct/seriesAtari.cc
  struct/seriesRaw.cc
  struct/level.cc
  struct/levelAtari.cc
  struct/levelRaw.cc
  struct/simpleLevel.cc
  struct/explosions.cc
  struct/blobDescr.cc
  struct/seriesManager.cc
  game/back.cc
  game/backFactory.cc
  game/ball.cc
  game/blob.cc
  game/bomb.cc
  game/breakablerock.cc
  game/burningBomb.cc
  game/button.cc
  game/closeDoor.cc
  game/conveyorBelt.cc
  game/conveyLever.cc
  game/detonator.cc
  game/diam.cc
  game/diamMonster.cc
  game/door.cc
  game/dynamite.cc
  game/eatable.cc
  game/elemFactory.cc
  game/fallingEatable.cc
  game/foregroundElem.cc
  game/game.cc
  game/gElem.cc
  game/generator.cc
  game/grassMonster.cc
  game/holeWall.cc
  game/hungryMonster.cc
  game/init.cc
  game/key.cc
  game/keyGen.cc
  game/leftMonster.cc
  game/lift.cc
  game/lock.cc
  game/miscEatable.cc
  game/openPortcullis.cc
  game/passingWall.cc
  game/permeabWall.cc
  game/player.cc
  game/playerServer.cc
  game/portcullis.cc
  game/random.cc
  game/rightMonster.cc
  game/rock.cc
  game/rockGen.cc
  game/rockMonster.cc
  game/roll.cc
  game/spring.cc
  game/telepWall.cc
  game/transformWall.cc
  game/triggerButton.cc
  game/triggerCover.cc
  game/vars.cc
  gui/qGameField.cc
  gui/gameField.cc
  gui/qglGameField.cc
  gui/qAbout.cc
  gui/qAboutMessages.cc
  gui/qRollAbout.cc
  gui/clientDialog.cc
  gui/scoreBox.cc
  gui/playField.cc
  gui/mainWin.cc
  gui/configWin.cc
  gui/gameConfig.cc
  gui/glcontext.cc
  gui/editPalette.cc
  gui/levelParams.cc
  gui/painter.cc
  gui/seriesArranger.cc
  gui/seriesIconView.cc
  gui/simpleLevelIO.cc
  somasound/diffcode.cc
  somasound/somaqsound.cc
  somasound/somaqsoundeffect.cc
  somasound/somasoundalsa.cc
  somasound/somasoundoss.cc
  somasound/somasoundprocessor.cc
  somasound/somasoundqtphonon.cc
  somasound/soundbank.cc
  somasound/sounditerator.cc
  somasound/wavheader.cc
  socket/netMessages.cc
  socket/playerServerSocket.cc
  socket/qPlayerServer.cc
  sound/rollsoundbank.cc
  socket/singleSocket.cc
)

set( _headers 
  "struct/atariFormat.h"
  "struct/blobDescr.h"
  "struct/explosions.h"
  "struct/general.h"
  "struct/levelAtari.h"
  "struct/level.h"
  "struct/levelParam.h"
  "struct/levelPar.h"
  "struct/levelRaw.h"
  "struct/qfileWrapper.h"
  "struct/seriesAtari.h"
  "struct/series.h"
  "struct/seriesManager.h"
  "struct/seriesRaw.h"
  "struct/simpleLevel.h"
  "struct/simpleLevel_d.h"
  "struct/stream_traits.h"
  "struct/workLevel.h"
  "game/back.h"
  "game/backFactory.h"
  "game/ball.h"
  "game/blob.h"
  "game/bomb.h"
  "game/breakablerock.h"
  "game/burningBomb.h"
  "game/button.h"
  "game/closeDoor.h"
  "game/conveyLever.h"
  "game/conveyorBelt.h"
  "game/constantes.h"
  "game/detonator.h"
  "game/diam.h"
  "game/diamMonster.h"
  game/documentation.h
  game/door.h
  game/dynamite.h
  game/eatable.h
  game/elemFactory.h
  game/fallingEatable.h
  game/foregroundElem.h
  game/game.h
  game/gElem.h
  game/generator.h
  game/grassMonster.h
  game/holeWall.h
  game/hungryMonster.h
  game/init.h
  game/key.h
  game/keyGen.h
  game/leftMonster.h
  game/lift.h
  game/lock.h
  game/miscEatable.h
  game/openPortcullis.h
  game/passingWall.h
  game/permeabWall.h
  game/portcullis.h
  game/player.h
  game/playerServer.h
  game/random.h
  game/rightMonster.h
  game/rock.h
  game/rockGen.h
  game/rockMonster.h
  game/roll.h
  game/spring.h
  game/spritFlag.h
  game/telepWall.h
  game/transformWall.h
  game/triggerButton.h
  game/triggerCover.h
  game/vars.h
  gui/gameField.h
  gui/qGameField.h
  gui/qglGameField.h
  gui/qAbout.h
  gui/qRollAbout.h
  gui/clientDialog.h
  gui/playField.h
  gui/scoreBox.h
  gui/mainWin.h
  gui/configWin.h
  gui/gameConfig.h
  gui/glcontext.h
  gui/editPalette.h
  gui/levelParams.h
  gui/painter.h
  gui/seriesArranger.h
  gui/seriesIconView.h
  gui/qdatastreamWrapper.h
  somasound/somaqsound.h
  somasound/somaqsoundeffect.h
  somasound/somasoundalsa.h
  somasound/somasoundoss.h
  somasound/somasoundprocessor.h
  somasound/somasoundqtphonon.h
  somasound/soundbank.h
  somasound/sounditerator.h
  somasound/soundslot.h
  somasound/wavheader.h
  sound/rollsoundbank.h
  sound/soundProcessor.h
  socket/qPlayerServer.h
  socket/playerServerSocket.h
  socket/singleSocket.h
  socket/netMessages.h
)

add_library( roll SHARED ${_sources} )
set_property( TARGET roll PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( roll ${QROLL_LIBRARIES} )
if( DESIRED_QT_VERSION EQUAL 5 )
#   qt5_use_modules( roll Widgets Network Multimedia OpenGL )
  target_link_libraries( roll ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES}
                         ${Qt5Multimedia_LIBRARIES} ${Qt5OpenGL_LIBRARIES} )
  set_property( TARGET roll APPEND PROPERTY INCLUDE_DIRECTORIES
                ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS}
                ${Qt5Multimedia_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} )
  set_property( TARGET roll APPEND PROPERTY
                COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG )
  set_property( TARGET roll APPEND PROPERTY
                COMPILE_DEFINITIONS_RELWITHDEBINFO QT_NO_DEBUG)
  set_property( TARGET roll APPEND PROPERTY
                COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG)
  if( Qt5_POSITION_INDEPENDENT_CODE
      AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
           OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0))
    set_property( TARGET roll PROPERTY POSITION_INDEPENDENT_CODE
                  ${Qt5_POSITION_INDEPENDENT_CODE} )
  endif()

endif()

if( brainvisa_enabled )

  BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers roll headersTarget )
  add_dependencies( roll ${headersTarget} )

  BRAINVISA_INSTALL( TARGETS roll
                DESTINATION lib
                COMPONENT ${PROJECT_NAME} )

else() # not brainvisa_enabled

  unset( headersTarget )
  foreach( _file ${_headers} )
    set( f "${CMAKE_BINARY_DIR}/include/roll/${_file}" )
    set( _absoluteFile "${CMAKE_SOURCE_DIR}/src/libroll/${_file}" )
    set( headersTarget "${headersTarget}" "${f}" )
    add_custom_command(
      OUTPUT "${f}"
      DEPENDS "${_absoluteFile}"
      COMMAND "${CMAKE_COMMAND}" -E copy "${_absoluteFile}" "${f}" )
  endforeach()
  add_custom_target( roll_headers ALL
    DEPENDS ${headersTarget} )

  add_dependencies( roll roll_headers )

endif() # not brainvisa_enabled
