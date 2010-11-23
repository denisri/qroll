
if( brainvisa_enabled )
  BRAINVISA_GET_FILE_LIST_FROM_PRO( libroll.pro "HEADERS" _headers "SOURCES" _sources )
  BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers roll headersTarget )
  BRAINVISA_ADD_MOC_FILES( _sources ${_headers} )

  add_library( roll SHARED ${_sources} )
  add_dependencies( roll ${headersTarget} )
  set_property( TARGET roll PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
  set( _libraries ${OPENGL_LIBRARIES} ${QT_LIBRARIES} )
  if(ALSA_FOUND)
    set( _libraries ${_libraries} ${ALSA_LIBRARIES} )
  endif()
  target_link_libraries( roll ${_libraries} )

  BRAINVISA_INSTALL( TARGETS roll
                DESTINATION lib
                COMPONENT ${PROJECT_NAME} )

else()

  set( _sources "struct/blobDescr.cc"
  )
  set( _headers "struct/atariFormat.h"
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
  )

  add_library( roll SHARED ${_sources} )
  set_property( TARGET roll PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
  target_link_libraries( roll ${LIBS} )

endif()
