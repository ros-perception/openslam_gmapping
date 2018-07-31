cmake_minimum_required(VERSION 3.5)
project(openslam_gmapping)

find_package(ament_cmake_auto REQUIRED)
find_package(ament_cmake_ros REQUIRED)

include_directories(
  include
  utils
  scanmatcher
  sensor/sensor_base)

# add_subdirectory(gridfastslam)
ament_auto_add_library(gridfastslam
  gridfastslam/gfsreader.cpp
  gridfastslam/gridslamprocessor.cpp
  gridfastslam/gridslamprocessor_tree.cpp
  gridfastslam/motionmodel.cpp
)
ament_target_dependencies(gridfastslam)

# add_subdirectory(scanmatcher)
ament_auto_add_library(scanmatcher
  scanmatcher/eig3.cpp
  scanmatcher/scanmatcher.cpp
  scanmatcher/scanmatcherprocessor.cpp
  scanmatcher/smmap.cpp
)
ament_target_dependencies(scanmatcher)

# add_subdirectory(sensor/sensor_base)
ament_auto_add_library(sensor_base
  sensor/sensor_base/sensor.cpp
  sensor/sensor_base/sensorreading.cpp
)
ament_target_dependencies(sensor_base)

# add_subdirectory(sensor/sensor_odometry)
ament_auto_add_library(sensor_odometry
  sensor/sensor_odometry/odometryreading.cpp
  sensor/sensor_odometry/odometrysensor.cpp
)
ament_target_dependencies(sensor_odometry)

# add_subdirectory(sensor/sensor_range)
ament_auto_add_library(sensor_range
  sensor/sensor_range/rangereading.cpp
  sensor/sensor_range/rangesensor.cpp
)
ament_target_dependencies(sensor_range)

# add_subdirectory(utils)
ament_auto_add_library(utils
  utils/movement.cpp
  utils/stat.cpp
)
ament_target_dependencies(utils)

ament_auto_package()
