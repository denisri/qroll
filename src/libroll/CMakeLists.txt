
if( NOT brainvisa_enabled )

  macro( BRAINVISA_ADD_MOC_FILES _sources )
    foreach( _current_FILE ${ARGN} )
      get_filename_component( _tmp_FILE ${_current_FILE} ABSOLUTE )
      file( READ "${_tmp_FILE}" _content )
      string( REGEX MATCH Q_OBJECT _match "${_content}" )
      if( _match )
        get_filename_component( _basename ${_tmp_FILE} NAME_WE )
        set( _moc ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.moc.cpp )
        if( DESIRED_QT_VERSION EQUAL 3 )
          add_custom_command(OUTPUT ${_moc}
            COMMAND ${QT_MOC_EXECUTABLE}
            ARGS ${_tmp_FILE} -o ${_moc}
            DEPENDS ${_tmp_FILE}
          )
        else()
          QT4_GENERATE_MOC( "${_tmp_FILE}" "${_moc}" )
        endif()
        set(${_sources} ${${_sources}} "${_moc}" )
      endif()
    endforeach()
  endmacro()

endif() # brainvisa_enabled

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
  game/dynamite.cc
  game/detonator.cc
  game/conveyLever.cc
  game/button.cc
  game/ball.cc
  game/portcullis.cc
  game/openPortcullis.cc
  game/player.cc
  game/playerServer.cc
  game/spring.cc
  game/lift.cc
  game/conveyorBelt.cc
  game/miscEatable.cc
  game/foregroundElem.cc
  game/telepWall.cc
  game/keyGen.cc
  game/bomb.cc
  game/burningBomb.cc
  game/generator.cc
  game/rockGen.cc
  game/diamMonster.cc
  game/rockMonster.cc
  game/grassMonster.cc
  game/hungryMonster.cc
  game/lock.cc
  game/key.cc
  game/fallingEatable.cc
  game/diam.cc
  game/eatable.cc
  game/holeWall.cc
  game/permeabWall.cc
  game/transformWall.cc
  game/passingWall.cc
  game/blob.cc
  game/rightMonster.cc
  game/leftMonster.cc
  game/closeDoor.cc
  game/door.cc
  game/rock.cc
  game/backFactory.cc
  game/back.cc
  game/elemFactory.cc
  game/roll.cc
  game/init.cc
  game/game.cc
  game/vars.cc
  game/gElem.cc
  game/random.cc
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
  "struct/seriesAtari.h"
  "struct/series.h"
  "struct/seriesManager.h"
  "struct/seriesRaw.h"
  "struct/simpleLevel.h"
  "struct/simpleLevel_d.h"
  "struct/workLevel.h"
  "game/ball.h"
  "game/button.h"
  "game/conveyLever.h"
  "game/conveyorBelt.h"
  game/detonator.h
  game/documentation.h
  game/dynamite.h
  game/portcullis.h
  game/openPortcullis.h
  game/player.h
  game/playerServer.h
  game/spring.h
  game/lift.h
  game/miscEatable.h
  game/foregroundElem.h
  game/telepWall.h
  game/keyGen.h
  game/bomb.h
  game/burningBomb.h
  game/generator.h
  game/rockGen.h
  game/diamMonster.h
  game/rockMonster.h
  game/grassMonster.h
  game/hungryMonster.h
  game/lock.h
  game/key.h
  game/fallingEatable.h
  game/diam.h
  game/eatable.h
  game/holeWall.h
  game/permeabWall.h
  game/transformWall.h
  game/passingWall.h
  game/blob.h
  game/rightMonster.h
  game/leftMonster.h
  game/closeDoor.h
  game/door.h
  game/rock.h
  game/backFactory.h
  game/back.h
  game/elemFactory.h
  game/roll.h
  game/spritFlag.h
  game/init.h
  game/constantes.h
  game/game.h
  game/vars.h
  game/gElem.h
  game/random.h
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

if( NOT DESIRED_QT_VERSION EQUAL 5 )
  BRAINVISA_ADD_MOC_FILES( _sources ${_headers} )
endif()

add_library( roll SHARED ${_sources} )
set_property( TARGET roll PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( roll ${QROLL_LIBRARIES} )
if( DESIRED_QT_VERSION EQUAL 5 )
  qt5_use_modules( roll Widgets Network Multimedia OpenGL )
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
