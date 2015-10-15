
if( brainvisa_enabled )
  brainvisa_add_executable( qrsndconv sndconv.cc )
else()
  add_executable( qrsndconv sndconv.cc )
endif()


