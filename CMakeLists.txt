cmake_minimum_required(VERSION 4.1)

project(OpenGL-Template VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message(STATUS "Executables will be placed in: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


message(STATUS "Searching for required libraries...")

set(OpenGL_GL_PREFERENCE GLVND)
message(STATUS "Set OpenGL preference to GLVND.")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

message(STATUS "Found all required libraries.")


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

message(STATUS "Linking libraries to ${PROJECT_NAME}...")
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets to build directory..."
)

if (APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
endif()

message(STATUS "Adding custom 'clean_all' target.")
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/CMakeCache.txt"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/cmake_install.cmake"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/compile_commands.json"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
    COMMENT "Cleaning the build directory..."
)


message(STATUS "CMake configuration complete.")
message(STATUS "To build:          cmake --build build")
message(STATUS "To clean:          cmake --build build --target clean_all")
message(STATUS "To clean and rebuild: cmake --build build --target clean_all && cmake --build build")
