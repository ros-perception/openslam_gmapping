include_directories(../include/gmapping)
add_library(log
    carmenconfiguration.cpp
    configuration.cpp
    log_plot.cpp
    sensorlog.cpp
    sensorstream.cpp
)
install(TARGETS log DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

add_executable(rdk2carmen rdk2carmen.cpp)
add_executable(scanstudio2carmen scanstudio2carmen.cpp)
add_executable(log_plot log_plot.cpp)
add_executable(log_test log_test.cpp)

target_link_libraries(rdk2carmen log sensor_odometry sensor_range)
target_link_libraries(scanstudio2carmen log sensor_odometry sensor_range)
target_link_libraries(log_plot log sensor_odometry sensor_range)
target_link_libraries(log_test log sensor_odometry sensor_range)

install(TARGETS rdk2carmen DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS scanstudio2carmen DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS log_plot DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS log_test DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
