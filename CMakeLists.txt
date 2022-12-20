cmake_minimum_required( VERSION 3.10 )

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

set( QROLL_DEFINITIONS )

if( NOT DESIRED_QT_VERSION )
  find_package( Qt REQUIRED )
endif()
if( DESIRED_QT_VERSION EQUAL 4 )
  find_package( Qt4 COMPONENTS QtCore QtGui QtOpenGL QtNetwork REQUIRED )
  # Qt3Support removed
  find_package( Qt4 COMPONENTS phonon )
  find_package( Qt4 COMPONENTS QtWebKit )
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
elseif( DESIRED_QT_VERSION EQUAL 6 )
  find_package( Qt6 REQUIRED COMPONENTS Widgets Network Multimedia OpenGL
                                        OpenGLWidgets )
  add_definitions( ${Qt6Core_DEFINITIONS} ${Qt6Widgets_DEFINITIONS}
                   ${Qt6OpenGL_DEFINITIONS} ${Qt6OpenGLWidgets_DEFINITIONS}
                   ${Qt6Network_DEFINITIONS} ${Qt6Multimedia_DEFINITIONS})
  include_directories( ${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS}
                       ${Qt6OpenGL_INCLUDE_DIRS}
                       ${Qt6OpenGLWidgets_INCLUDE_DIRS}
                       ${Qt6Network_INCLUDE_DIRS}
                       ${Qt6Multimedia_INCLUDE_DIRS} )
  set( QT_LIBRARIES Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets Qt6::Network
                    Qt6::Multimedia )
  list( APPEND QROLL_DEFINITIONS "-DRR_NO_OPENGL" )  # fot now: FIXME
else()
  # Qt > 6
  message( FATAL_ERROR "Qt > 6 is not supported yet" )
#   find_package( QtVersion )
#   find_package( OpenGL )
endif()

find_package(ALSA) # system cmake
if( NOT ALSA_FOUND AND brainvisa_enabled )
  find_package(Alsa) # brainvisa
endif()

list( APPEND QROLL_DEFINITIONS "-DQROLL_EXPORTS" "-DHAS_GLEXT" )
set( QROLL_LIBRARIES ${OPENGL_LIBRARIES} )

if( NOT OPENGL_FOUND )
  lit( APPEND QROLL_DEFINITIONS "-DRR_NO_OPENGL" )
endif()

if (EXISTS '/usr/include/sys/soundcard.h' OR EXISTS '/usr/include/sys/audioio.h' )
  list( APPEND QROLL_DEFINITIONS "-DSOMA_SOUND_OSS" )
endif()
if(ALSA_FOUND)
  include_directories( "${ALSA_INCLUDE_DIR}" )
  if( brainvisa_enabled )
    BRAINVISA_DEPENDENCY(RUN DEPENDS libasound2 RUN)
  endif()
  list( APPEND QROLL_DEFINITIONS "-DSOMA_SOUND_ALSA" )
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${ALSA_LIBRARIES} )
endif()
if( QT_PHONON_FOUND )
#   list( APPEND QROLL_DEFINITIONS "-DSOMA_SOUND_QTPHONON" )
# QT_PHONON_LIB is already defined in FindQt4
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${QT_PHONON_LIBRARY} )
else()
  message( "Phonon has not been found. Phonon sound system will be disabled in QRoll." )
endif()
if( QT_QTWEBKIT_FOUND )
  set( QROLL_LIBRARIES ${QROLL_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} )
else()
  list( APPEND QROLL_DEFINITIONS "-DNO_WEBKIT" )
endif()

add_definitions( ${QROLL_DEFINITIONS} )

set( QROLL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include )
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

  # installer part
  BRAINVISA_FIND_PACKAGE( brainvisa-installer QUIET )
  if( brainvisa-installer_FOUND )
    if( WIN32 )
      add_custom_target( qroll-icons
        "${CMAKE_COMMAND}" -E copy_file "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/rockroll.xpm"
        COMMAND "${CMAKE_COMMAND}" -E copy_file "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/qroll_logo.xpm"
        COMMAND "${CMAKE_COMMAND}" -E copy_file "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/qroll_watermark.xpm"
      )
    else()
      add_custom_target( qroll-icons
        "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/rockroll.xpm"
        COMMAND "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/qroll_logo.xpm"
        COMMAND "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/share/icons/rockroll.xpm" "${CMAKE_BINARY_DIR}/share/brainvisa/installer/images/qroll_watermark.xpm"
      )
    endif()
    add_dependencies( qroll qroll-icons )
  endif()

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

