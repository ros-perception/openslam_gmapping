include_directories(./ ../include/gmapping)
add_library(configfile configfile.cpp)
install(TARGETS configfile DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
