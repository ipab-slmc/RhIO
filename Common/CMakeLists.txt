###
### Rhoban Input Output Library: Common part
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(rhio_common)

find_package(catkin REQUIRED)

# What is published by the 'rhio_common package'
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rhio_common
)

#Include directories
include_directories(include/)

#Sources files
set(SOURCES
    src/DataBuffer.cpp
    src/Protocol.cpp
    src/Logging.cpp
)

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Common as shared library
add_library(rhio_common SHARED ${SOURCES})

#Install rules
install(
    DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
install(
    TARGETS rhio_common 
    DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

