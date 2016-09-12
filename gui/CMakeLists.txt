include_directories(.. ../include/gmapping ../include/gmapping/log)

set(CMAKE_CXX_FLAGS -fPIC)
set(CMAKE_EXE_LINKER_FLAGS -fPIE)
set(CMAKE_AUTOMOC ON)

add_library(gui
    gsp_thread.cpp
    qgraphpainter.cpp
    qmappainter.cpp
    qnavigatorwidget.cpp
    qparticleviewer.cpp
    qpixmapdumper.cpp
    qslamandnavwidget.cpp
)

find_package(Qt5Widgets)
if(Qt5Widgets_FOUND)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
  target_link_libraries(gui Qt5::Widgets)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  target_link_libraries(gui ${QT_LIBRARIES})
endif()
include_directories(${CMAKE_BINARY_DIR}/gui)

add_executable(gfs_simplegui gfs_simplegui.cpp)
add_executable(gfs_nogui gfs_nogui.cpp)
add_executable(gfs2img gfs2img.cpp)
add_executable(gfs_logplayer gfs_logplayer.cpp)

find_package (Threads REQUIRED)
target_link_libraries(gfs_simplegui ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gfs_nogui ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gfs2img ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(gfs_simplegui gui gridfastslam configfile log sensor_odometry)
target_link_libraries(gfs_nogui gui gridfastslam configfile log sensor_odometry)
target_link_libraries(gfs2img gui gridfastslam configfile log sensor_odometry)
target_link_libraries(gfs_logplayer gui gridfastslam configfile log sensor_odometry)

install(TARGETS gfs_simplegui DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS gfs_nogui DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS gfs2img DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(TARGETS gfs_logplayer DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

