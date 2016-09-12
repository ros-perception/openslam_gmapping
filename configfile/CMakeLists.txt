include_directories(../include/gmapping)
add_library(configfile configfile.cpp)
install(TARGETS configfile DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

add_executable(configfile_test configfile_test.cpp)
target_link_libraries(configfile_test configfile)
install(TARGETS configfile_test DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
