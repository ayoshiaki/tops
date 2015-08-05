project(ToPS)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost 1.50 COMPONENTS regex REQUIRED)
find_package(Boost 1.50 COMPONENTS program_options REQUIRED)
find_package(Boost 1.50 COMPONENTS date_time REQUIRED)
find_package(Boost 1.50 COMPONENTS system REQUIRED)
find_package(Boost 1.50 COMPONENTS filesystem REQUIRED)

# find_package(PerlLibs)
# ADD_DEFINITIONS("-O0 -Wall -gdwarf-2")
ADD_DEFINITIONS(" -O3 -finline-functions -funroll-loops -ffast-math  -g ")


IF(UNIX)
  SET(BOOST_REGEX_LIBRARY ${Boost_REGEX_LIBRARY})
  SET(BOOST_PROGRAM_OPTIONS_LIBRARY ${Boost_PROGRAM_OPTIONS_LIBRARY})
  SET(BOOST_DATE_TIME_LIBRARY ${Boost_DATE_TIME_LIBRARY})
  FIND_LIBRARY(BOOST_REGEX_LIBRARY boost_regex-mt PATHS ${Boost_LIBRARY_DIRS} )
  FIND_LIBRARY(BOOST_DATE_TIME_LIBRARY boost_date_time-mt PATHS ${Boost_LIBRARY_DIRS} )
  FIND_LIBRARY(BOOST_PROGRAM_OPTIONS_LIBRARY boost_program_options-mt PATHS ${Boost_LIBRARY_DIRS} )

ENDIF(UNIX)


# add_subdirectory(googletest)
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(lang)

# enable_testing()
