cmake_minimum_required( VERSION 2.6 )

find_package( brainvisa-cmake )
message( 'roploploff1' )
if( 0 ) #brainvisa-cmake_FOUND )
  message( 'roploploff brainvisa' )
  set( brainvisa_enabled "1" )
  BRAINVISA_PROJECT()
  message( 'roploploff brainvisa done' )

else()

  set( brainvisa_enabled "" )
  set( PROJECT_NAME qroll )
  set( qroll_VERSION 0.7 )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  if ( NOT DEFINED WIN32 )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
  else()
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  endif()
endif()
message( 'roploploff2' )

if( NOT DESIRED_QT_VERSION )
  find_package( Qt REQUIRED )
endif()
if( DESIRED_QT_VERSION EQUAL 4 )
  find_package( Qt4 COMPONENTS QtCore QtGui Qt3Support QtOpenGL REQUIRED )
endif()
find_package( QtVersion )
find_package( OpenGL REQUIRED )

set( QROLL_DEFINITIONS "-DQROLL_EXPORTS" "-DHAS_GLEXT" )

set( QROLL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include )
include_directories( "${QROLL_INCLUDE_DIR}" "${QT_QTCORE_INCLUDE_DIR}" "${QT_QTGUI_INCLUDE_DIR}" "${QT_QTOPENGL_INCLUDE_DIR}" "${QT_QT3SUPPORT_INCLUDE_DIR}" )
set( QROL_LIBS "${QT_QT3SUPPORT_LIBRARY}" "${QT_QTOPENGL_LIBRARY}" "${QT_QTGUI_LIBRARY}" "${QT_QTCORE_LIBRARY}" )

find_package(ALSA) # system cmake
if( NOT ALSA_FOUND AND brainvisa_enabled )
  find_package(Alsa) # brainvisa
endif()

if (EXISTS '/usr/include/sys/soundcard.h' OR EXISTS '/usr/include/sys/audioio.h' )
  set(QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_OSS" )
endif()
if(ALSA_FOUND)
  include_directories( "${ALSA_INCLUDE_DIR}" )
  if( brainvisa_enabled )
    BRAINVISA_DEPENDENCY(RUN DEPENDS libasound2 RUN)
  endif()
  set(QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_ALSA")
endif()

add_definitions( ${QROLL_DEFINITIONS} )

set( QROLL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include )
include_directories( "${QROLL_INCLUDE_DIR}" )

if( DESIRED_QT_VERSION EQUAL 3 )
  include_directories( ${QT_INCLUDE_DIR} )
  add_definitions( ${QT_DEFINITIONS} )
else()
  include( "${QT_USE_FILE}" )
endif()

add_subdirectory( src )
# add_subdirectory( doxygen )

if( brainvisa_enabled )
  # qt translation files
#   BRAINVISA_ADD_TRANSLATION( "share" "share/qroll-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" ${PROJECT_NAME})

#   BRAINVISA_COPY_DIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/share
#                             share/qroll-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}
#                             ${PROJECT_NAME} )

  # BRAINVISA_CREATE_CMAKE_CONFIG_FILES()

else()

#   set( QROLL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
#   include_directories( ${QROLL_INCLUDE_DIR} )

endif()

