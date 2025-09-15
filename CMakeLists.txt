# =============================================================================
# CMAKE PROJECT CONFIGURATION
# =============================================================================
cmake_minimum_required(VERSION 4.1)
project(OpenGLProject VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
# DEPENDENCY MANAGEMENT
# =============================================================================
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

# --- AUTOMATED: GLFW & GLM ---
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glfw glm)

# --- LOCAL: GLAD ---
# This includes the 'glad' sub-project using its own CMakeLists.txt
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad)
message(STATUS "Added local GLAD from external/glad")

# =============================================================================
# FIND SYSTEM LIBRARIES
# =============================================================================
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# =============================================================================
# DEFINE THE EXECUTABLE
# =============================================================================
# It's more robust to list sources explicitly.
set(PROJECT_SOURCES
    src/Main.cpp
    # src/App.cpp
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# =============================================================================
# CONFIGURE TARGET PROPERTIES
# =============================================================================
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    # GLM is header-only, just need its include path
    ${glm_SOURCE_DIR}
)

message(STATUS "Linking libraries to ${PROJECT_NAME}...")
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glad
)

# (The rest of the file, like asset management, is mostly okay, but I've fixed the clean target)
# =============================================================================
# CUSTOM CLEAN TARGET
# =============================================================================
add_custom_target(clean_all
    COMMENT "Cleaning the build directory..."
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/CMakeFiles"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/_deps" # Where FetchContent downloads
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/CMakeCache.txt"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/cmake_install.cmake"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/compile_commands.json"
    COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
)
# ...
