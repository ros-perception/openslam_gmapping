add_library(sensor_odometry odometryreading.cpp odometrysensor.cpp)
target_link_libraries(sensor_odometry sensor_base)

install(TARGETS sensor_odometry DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
