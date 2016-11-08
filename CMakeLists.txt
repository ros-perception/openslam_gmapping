cmake_minimum_required(VERSION 2.8)
project(openslam_gmapping)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES gridfastslam scanmatcher sensor_base sensor_range sensor_odometry utils
)

include_directories(include)

add_subdirectory(gridfastslam)
add_subdirectory(scanmatcher)
add_subdirectory(sensor)
add_subdirectory(utils)

if(0)
  add_subdirectory(gui)
endif()

install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
                 PATTERN "*.hxx"
  PATTERN ".svn" EXCLUDE
)
