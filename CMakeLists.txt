cmake_minimum_required( VERSION 2.6 )

set(  brainvisa_enabled "0" )
set( USE_BRAINVISA_CMAKE TRUE CACHE BOOL "use Brainvisa cmake build system" )

if( USE_BRAINVISA_CMAKE )
  find_package( brainvisa-cmake )
  if( brainvisa-cmake_FOUND )
    set( brainvisa_enabled "1" )
    BRAINVISA_PROJECT()
  endif()
endif()

if( NOT brainvisa_enabled )
  set( PROJECT_NAME qroll )
  set( qroll_VERSION 1.0 )
  set( QROLL_VERSION_MAJOR 1 )
  set( QROLL_VERSION_MINOR 0 )
  set( QROLL_VERSION_PATCH 0 )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  if ( NOT DEFINED WIN32 )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
  else()
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  endif()
endif()

if( NOT DESIRED_QT_VERSION )
  find_package( Qt REQUIRED )
endif()
if( DESIRED_QT_VERSION EQUAL 4 )
  find_package( Qt4 COMPONENTS QtCore QtGui QtOpenGL QtNetwork REQUIRED )
  # Qt3Support removed
  find_package( Qt4 COMPONENTS phonon )
#   find_package( QtVersion )
  find_package( OpenGL )
  if( brainvisa_enabled )
    BRAINVISA_DEPENDENCY( RUN DEPENDS libqtcore4 RUN ">= 4.6" ) # ${QTVERSION}
    BRAINVISA_DEPENDENCY( DEV DEPENDS libqtcore4 DEV )
    BRAINVISA_DEPENDENCY( RUN DEPENDS libqtgui4 RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV DEPENDS libqtgui4 DEV )
    # BRAINVISA_DEPENDENCY( RUN DEPENDS libqt4-qt3support RUN ">= ${QT_VERSION}" )
    # BRAINVISA_DEPENDENCY( DEV DEPENDS libqt4-qt3support DEV )
    BRAINVISA_DEPENDENCY( RUN DEPENDS libqt4-opengl RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV DEPENDS libqt4-opengl DEV )
    # those seem to be loaded indirectly, I don't really know how
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-assistantclient RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-assistantclient DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-clucene RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-clucene DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-designer RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-designer DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-help RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-help DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-multimedia RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-multimedia DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-script RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-script DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-scripttools RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-scripttools DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-svg RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-svg DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-test RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-test DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-webkit RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-webkit DEV )
    BRAINVISA_DEPENDENCY( RUN RECOMMENDS libqt4-xmlpatterns RUN ">= 4.6" )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS libqt4-xmlpatterns DEV )
  endif()
elseif( DESIRED_QT_VERSION EQUAL 5 )
  find_package( Qt5Widgets REQUIRED )
  find_package( Qt5Network REQUIRED )
  find_package( Qt5Multimedia REQUIRED )
  find_package( OpenGL REQUIRED )
  find_package( Qt5OpenGL REQUIRED )
  if( brainvisa_enabled )
    # TODO: packaging Qt5
  endif()
else()
  # Qt3
  message( FATAL_ERROR "Qt3 is not supported any longer" )
#   find_package( QtVersion )
#   find_package( OpenGL )
endif()

find_package(ALSA) # system cmake
if( NOT ALSA_FOUND AND brainvisa_enabled )
  find_package(Alsa) # brainvisa
endif()

set( QROLL_DEFINITIONS "-DQROLL_EXPORTS" "-DHAS_GLEXT" )
set( QROLL_LIBRARIES ${OPENGL_LIBRARIES} )

if( NOT OPENGL_FOUND )
  set( QROLL_DEFINITIONS "-DRR_NO_OPENGL" )
endif()

if (EXISTS '/usr/include/sys/soundcard.h' OR EXISTS '/usr/include/sys/audioio.h' )
  set( QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_OSS" )
endif()
if(ALSA_FOUND)
  include_directories( "${ALSA_INCLUDE_DIR}" )
  if( brainvisa_enabled )
    BRAINVISA_DEPENDENCY(RUN DEPENDS libasound2 RUN)
  endif()
  set( QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_ALSA" )
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${ALSA_LIBRARIES} )
endif()
if( QT_PHONON_FOUND )
#   set( QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_QTPHONON" )
# QT_PHONON_LIB is already defined in FindQt4
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${QT_PHONON_LIBRARY} )
else()
  message( "Phonon has not been found. Phonon sound system will be disabled in QRoll." )
endif()

add_definitions( ${QROLL_DEFINITIONS} )

set( QROLL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include )
include_directories( "${QROLL_INCLUDE_DIR}" )

if( DESIRED_QT_VERSION EQUAL 3 )
  include_directories( ${QT_INCLUDE_DIR} )
  add_definitions( ${QT_DEFINITIONS} )
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${QT_LIBRARIES} )
elseif( DESIRED_QT_VERSION EQUAL 4 )
  include( "${QT_USE_FILE}" )
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} # ${QT_QT3SUPPORT_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} 
    ${QT_QTNETWORK_LIBRARY}
  )
else()
  # Qt 5
  set(CMAKE_AUTOMOC ON)
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} )
  add_definitions( ${Qt5Widgets_DEFINITIONS} ${Qt5Network_DEFINITIONS} 
    ${Qt5Multimedia_DEFINITIONS} ${Qt5OpenGL_DEFINITIONS} )
  include_directories( ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif()

add_subdirectory( src )
# add_subdirectory( doxygen )

if( brainvisa_enabled )
  # qt translation files
#   BRAINVISA_ADD_TRANSLATION( "share" "share/qroll-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" ${PROJECT_NAME})

  BRAINVISA_COPY_DIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/share
                            share/qroll-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}
                            ${PROJECT_NAME} )

  # BRAINVISA_CREATE_CMAKE_CONFIG_FILES()

else()

#   set( QROLL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
#   include_directories( ${QROLL_INCLUDE_DIR} )

  file( GLOB_RECURSE _sharedfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/share
    ${CMAKE_CURRENT_SOURCE_DIR}/share/* )
  foreach( _filename ${_sharedfiles} )
    configure_file( share/${_filename} 
      ${CMAKE_BINARY_DIR}/share/qroll-${QROLL_VERSION_MAJOR}.${QROLL_VERSION_MINOR}/${_filename} COPYONLY )
  endforeach()

endif()

