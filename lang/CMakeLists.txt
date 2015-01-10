project(tops-lang)

cmake_minimum_required(VERSION 2.6)

IF(UNIX)
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${RMATH_INCLUDE_DIR} )
ENDIF(UNIX)

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

IF(UNIX)
  SET(TOPS_DEP ${BOOST_REGEX_LIBRARY} ${RMATH_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
ENDIF(UNIX)

#add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)

enable_testing()
