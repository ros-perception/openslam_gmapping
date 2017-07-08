ament_auto_add_library(scanmatcher
  eig3.cpp
  scanmatcher.cpp
  scanmatcherprocessor.cpp
  smmap.cpp
)
ament_target_dependencies(scanmatcher)
