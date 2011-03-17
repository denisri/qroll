set( _targetname qroll )
if( WIN32 )
  set( _guiExecutable WIN32 )
endif( WIN32 )
if( APPLE )
  set( _guiExecutable MACOSX_BUNDLE )
#  set( _targetname qroll-bin )
endif( APPLE )

add_executable( ${_targetname} ${_guiExecutable} main.cc )
target_link_libraries( ${_targetname} roll ${QROLL_LIBRARIES} )

if( brainvisa_enabled )
  BRAINVISA_INSTALL( TARGETS ${_targetname}
    DESTINATION bin
    COMPONENT ${PROJECT_NAME} )
endif()

