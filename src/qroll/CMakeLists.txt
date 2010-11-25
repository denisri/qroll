
add_executable( qroll main.cc )
add_dependencies( qroll roll )
set( _libraries roll ${OPENGL_LIBRARIES} ${QT_LIBRARIES} )
if(ALSA_FOUND)
  set( _libraries ${_libraries} ${ALSA_LIBRARIES} )
endif()
target_link_libraries( qroll ${_libraries} )

