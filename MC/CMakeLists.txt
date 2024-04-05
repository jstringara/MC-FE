add_library(MC MC.cpp)

# import the necessary libraries
include_directories(${CMAKE_SOURCE_DIR}/model)
include_directories(${CMAKE_SOURCE_DIR}/option)
include_directories(${CMAKE_SOURCE_DIR}/matrix)
include_directories(${CMAKE_SOURCE_DIR}/vec)
