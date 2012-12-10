project(ToPS)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost 1.35 COMPONENTS regex)
find_package(Boost 1.35 COMPONENTS program_options)

# find_package(PerlLibs)
# ADD_DEFINITIONS("-Wall -g")

ADD_DEFINITIONS("-Wall -O3 -finline-functions -funroll-loops -ftree-vectorize -ffast-math  -g -O3")


IF(UNIX)
  SET(BOOST_REGEX_LIBRARY ${Boost_REGEX_LIBRARY})
  SET(BOOST_PROGRAM_OPTIONS_LIBRARY ${Boost_PROGRAM_OPTIONS_LIBRARY})
  FIND_LIBRARY(BOOST_REGEX_LIBRARY boost_regex-mt PATHS ${Boost_LIBRARY_DIRS} )
  FIND_LIBRARY(BOOST_PROGRAM_OPTIONS_LIBRARY boost_program_options-mt PATHS ${Boost_LIBRARY_DIRS} )
ENDIF(UNIX)



add_subdirectory(src)
add_subdirectory(app)



