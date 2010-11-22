
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
