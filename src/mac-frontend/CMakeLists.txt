if( WIN32 )
  set( _guiExecutable WIN32 )
endif( WIN32 )
if( APPLE )
  set( _guiExecutable MACOSX_BUNDLE )
endif( APPLE )

add_executable( qroll ${_guiExecutable} qroll.cc )
target_link_libraries( qroll roll ${QROLL_LIBRARIES} )

if( brainvisa_enabled )
  BRAINVISA_INSTALL( TARGETS qroll
    DESTINATION bin
    COMPONENT ${PROJECT_NAME} )
endif()

