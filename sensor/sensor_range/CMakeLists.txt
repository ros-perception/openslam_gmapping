include_directories(./)
add_library(sensor_range rangereading.cpp rangesensor.cpp)
install(TARGETS sensor_range DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
