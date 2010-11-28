
add_executable( qroll main.cc )
target_link_libraries( qroll roll ${QROLL_LIBRARIES} )

if( brainvisa_enabled )
  BRAINVISA_INSTALL( TARGETS qroll
    DESTINATION bin
    COMPONENT ${PROJECT_NAME} )
endif()

