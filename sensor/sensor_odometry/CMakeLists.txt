include_directories(./)
add_library(sensor_odometry odometryreading.cpp odometrysensor.cpp)
install(TARGETS sensor_odometry DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
