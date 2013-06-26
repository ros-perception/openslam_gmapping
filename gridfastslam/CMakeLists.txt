include_directories(./)
add_library(gridfastslam gfsreader.cpp gridslamprocessor.cpp gridslamprocessor_tree.cpp motionmodel.cpp)
install(TARGETS gridfastslam DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
