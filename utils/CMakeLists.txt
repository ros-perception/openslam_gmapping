include_directories(./)
add_library(utils movement.cpp stat.cpp)
install(TARGETS utils DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
