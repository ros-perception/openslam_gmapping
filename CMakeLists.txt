cmake_minimum_required(VERSION 3.5)
project(openslam_gmapping)

find_package(ament_cmake_auto REQUIRED)
find_package(ament_cmake_ros REQUIRED)

include_directories(include)

add_subdirectory(gridfastslam)
add_subdirectory(scanmatcher)
add_subdirectory(sensor)
add_subdirectory(utils)

ament_auto_package()
