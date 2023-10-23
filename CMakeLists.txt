project(ToPS)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 COMPONENTS regex REQUIRED)
find_package(Boost 1.50 COMPONENTS program_options REQUIRED)
find_package(Boost 1.50 COMPONENTS date_time REQUIRED)
find_package(Boost 1.50 COMPONENTS timer REQUIRED)
find_package(Boost 1.50 COMPONENTS system REQUIRED)
find_package(Boost 1.50 COMPONENTS filesystem REQUIRED)
find_package(Boost REQUIRED chrono)

# LIBTORCH package
set(CMAKE_PREFIX_PATH "/home/edisone/Downloads/libtorch")
find_package(Torch REQUIRED)
include_directories(${TORCH_INCLUDE_DIRS})

# Set Linker flags
SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
IF(LINUX)
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -lrt")
ENDIF()

# Find_package(PerlLibs)
# ADD_DEFINITIONS("-O0 -Wall -gdwarf-2")
add_definitions("-Ofast -funroll-loops -ffast-math")

if(UNIX)
  set(BOOST_REGEX_LIBRARY ${Boost_REGEX_LIBRARY})
  set(BOOST_PROGRAM_OPTIONS_LIBRARY ${Boost_PROGRAM_OPTIONS_LIBRARY})
  set(BOOST_DATE_TIME_LIBRARY ${Boost_DATE_TIME_LIBRARY})
  set(BOOST_TIMER_LIBRARY ${Boost_TIMER_LIBRARY})
  set(BOOST_CHRONO_LIBRARY ${Boost_CHRONO_LIBRARY})
  find_library(BOOST_REGEX_LIBRARY boost_regex-mt PATHS ${Boost_LIBRARY_DIRS} )
  find_library(BOOST_DATE_TIME_LIBRARY boost_date_time-mt PATHS ${Boost_LIBRARY_DIRS} )
  find_library(BOOST_PROGRAM_OPTIONS_LIBRARY boost_program_options-mt PATHS ${Boost_LIBRARY_DIRS} )
endif(UNIX)

# DEBUG
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(lang)


# ignore BOOST deprecated headers
  add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
  add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")
  add_definitions("-BOOST_TIMER_ENABLE_DEPRECATED")
