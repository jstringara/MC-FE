cmake_minimum_required(VERSION 3.10)

project(MonteCarlo CXX)

# Generate compile_commands.json (for clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(MonteCarlo main.cpp)

add_subdirectory(vec)
add_subdirectory(matrix)
add_subdirectory(model)
add_subdirectory(option)
add_subdirectory(MC)

target_link_libraries(MonteCarlo PUBLIC vec)
target_link_libraries(MonteCarlo PUBLIC matrix)
target_link_libraries(MonteCarlo PUBLIC model)
target_link_libraries(MonteCarlo PUBLIC option)
target_link_libraries(MonteCarlo PUBLIC MC)

target_include_directories(MonteCarlo PUBLIC 
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/vec"
    "${PROJECT_SOURCE_DIR}/matrix"
    "${PROJECT_SOURCE_DIR}/model"
    "${PROJECT_SOURCE_DIR}/option"
    "${PROJECT_SOURCE_DIR}/MC"
)


