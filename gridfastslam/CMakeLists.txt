if(OPENMP_FOUND)
  message(STATUS "OPENMP FOUND")
  set(OpenMP_LIBS gomp)
endif()

add_library(gridfastslam
    gfsreader.cpp
    gridslamprocessor.cpp
    gridslamprocessor_tree.cpp
    motionmodel.cpp
)

target_link_libraries(gridfastslam scanmatcher sensor_range ${OpenMP_LIBS})

install(TARGETS gridfastslam DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
