include_directories(./)
add_library(scanmatcher eig3.cpp scanmatcher.cpp scanmatcherprocessor.cpp smmap.cpp)
install(TARGETS scanmatcher DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
