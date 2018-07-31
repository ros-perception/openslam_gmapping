ament_auto_add_library(gridfastslam
    gfsreader.cpp
    gridslamprocessor.cpp
    gridslamprocessor_tree.cpp
    motionmodel.cpp
)
ament_target_dependencies(gridfastslam)
