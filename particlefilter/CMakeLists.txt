include_directories(../include/gmapping)

add_executable(particlefilter_test particlefilter_test.cpp)
add_executable(range_bearing range_bearing.cpp)

install(TARGETS particlefilter_test DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS range_bearing DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
