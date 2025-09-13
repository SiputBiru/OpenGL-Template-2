# =============================================================================
# CMAKE PROJECT CONFIGURATION
# =============================================================================
cmake_minimum_required(VERSION 4.1)
project(OpenGLProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
# DEPENDENCY MANAGEMENT WITH FETCHCONTENT
# =============================================================================
include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)
message(STATUS "Setting dependency download directory to: ${FETCHCONTENT_BASE_DIR}")

# --- GLFW ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# --- GLM ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)

FetchContent_MakeAvailable(glfw glm)

# =============================================================================
# FIND SYSTEM LIBRARIES
# =============================================================================
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
message(STATUS "Found required system libraries.")

# GLEW
# set(GLEW_CMAKELIST_PATH "${CMAKE_SOURCE_DIR}/external/glew/build/cmake/CMakeLists.txt")
# if(NOT EXISTS "${GLEW_CMAKELIST_PATH}")
# message(FATAL_ERROR "GLEW source not found. Please download and extract GLEW to '${CMAKE_SOURCE_DIR}/external/glew'.")
#
# endif()

find_package(GLEW REQUIRED)

message(STATUS "Found local GLEW source. Adding it to the build.")
# Add the GLEW build script to our project. Note the path is to the 'cmake' subfolder.
# add_subdirectory(${CMAKE_SOURCE_DIR}/external/glew/build/cmake)

# =============================================================================
# DEFINE THE EXECUTABLE
# =============================================================================
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message(STATUS "Executables will be placed in: ${CMAKE_BINARY_DIR}")

# =============================================================================
# CONFIGURE TARGET PROPERTIES (INCLUDES, DEFINITIONS, LIBRARIES)
# =============================================================================
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    ${glm_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

message(STATUS "Linking libraries to ${PROJECT_NAME}...")
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm
)

# =============================================================================
# ASSET MANAGEMENT
# =============================================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets to build directory..."
)

# =============================================================================
# CUSTOM CLEAN TARGET
# =============================================================================
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/external"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/CMakeCache.txt"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/cmake_install.cmake"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/compile_commands.json"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
    COMMENT "Cleaning the build directory..."
)

# =============================================================================
# COMPLETION MESSAGE
# =============================================================================
message(STATUS "CMake configuration complete.")
message(STATUS "To build:          cmake --build build")
message(STATUS "To clean:          cmake --build build --target clean_all")
