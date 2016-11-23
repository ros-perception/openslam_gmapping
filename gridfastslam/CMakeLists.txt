find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
  set(OpenMP_LIBS gomp)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_library(gridfastslam
    gfsreader.cpp
    gridslamprocessor.cpp
    gridslamprocessor_tree.cpp
    motionmodel.cpp
)

target_link_libraries(gridfastslam scanmatcher sensor_range ${OpenMP_LIBS})

install(TARGETS gridfastslam DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
