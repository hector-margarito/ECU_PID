cmake_minimum_required (VERSION 2.8)
project(counter_17bits)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH}/cmake-files ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(INSTALL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

find_package(SystemC REQUIRED)
find_package(SystemCAMS REQUIRED)

include_directories(${SYSTEMC_INCLUDE_DIR})
include_directories(${SYSTEMCAMS_INCLUDE_DIR})

add_subdirectory(src)

