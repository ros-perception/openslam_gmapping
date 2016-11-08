add_library(gridfastslam
    gfsreader.cpp
    gridslamprocessor.cpp
    gridslamprocessor_tree.cpp
    motionmodel.cpp
)
target_link_libraries(gridfastslam scanmatcher sensor_range)

install(TARGETS gridfastslam DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
