cmake_minimum_required( VERSION 2.6 )

find_package( brainvisa-cmake )
message( 'roploploff1' )
if( brainvisa-cmake_FOUND )
  message( 'roploploff brainvisa' )
  BRAINVISA_PROJECT()
endif()
message( 'roploploff2' )

if( NOT DESIRED_QT_VERSION )
  find_package( Qt REQUIRED )
endif()
if( DESIRED_QT_VERSION EQUAL 4 )
  find_package( Qt4 COMPONENTS QtCore QtGui Qt3Support QtOpenGL REQUIRED )
endif()
find_package( QtVersion ) #REQUIRED )
find_package( OpenGL REQUIRED )

set( QROLL_DEFINITIONS "-DQROLL_EXPORTS" "-DHAS_GLEXT" )

find_package(Alsa)
if (EXISTS '/usr/include/sys/soundcard.h' OR EXISTS '/usr/include/sys/audioio.h' )
  set(QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_OSS" )
endif()
if(ALSA_FOUND)
  if( brainvisa-cmake_FOUND )
    BRAINVISA_DEPENDENCY(RUN DEPENDS libasound2 RUN)
  endif()
  set(QROLL_DEFINITIONS "${QROLL_DEFINITIONS} -DSOMA_SOUND_ALSA")
endif()

add_definitions( ${QROLL_DEFINITIONS} )

add_subdirectory( src )
add_subdirectory( doxygen )

if( brainvisa-cmake_FOUND )
  # qt translation files
  BRAINVISA_ADD_TRANSLATION( "shared" "share/anatomist-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" ${PROJECT_NAME})

  BRAINVISA_COPY_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/doc"
                            "share/doc/anatomist-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
                            ${PROJECT_NAME}-doc )
  BRAINVISA_COPY_DIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/shared
                            share/anatomist-${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}
                            ${PROJECT_NAME} )

  # docbook documentation
  BRAINVISA_GENERATE_DOCBOOK_DOC()

  BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
endif()
