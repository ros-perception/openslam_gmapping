include_directories(./)
add_library(sensor_base sensor.cpp sensorreading.cpp)
install(TARGETS sensor_base DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
