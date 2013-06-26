cmake_minimum_required(VERSION 2.8)
project(gmapping)

find_package(catkin)

catkin_package(
  INCLUDE_DIRS ./include/
  LIBRARIES gridfastslam scanmatcher sensor_base sensor_range sensor_odometry utils
)

include_directories(./include/)

#add_subdirectory(log)
#add_subdirectory(configfile)
#add_subdirectory(carmenwrapper)
#add_subdirectory(gui)
#add_subdirectory(gfs-carmen)

add_subdirectory(gridfastslam)
add_subdirectory(scanmatcher)
add_subdirectory(sensor)
add_subdirectory(utils)
