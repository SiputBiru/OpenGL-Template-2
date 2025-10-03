# =============================================================================
# CMAKE PROJECT CONFIGURATION
# =============================================================================
cmake_minimum_required(VERSION 4.1)
project(OpenGLProject VERSION 0.1 LANGUAGES CXX C)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Local stb image
add_subdirectory(${CMAKE_SOURCE_DIR}/external/stb)
message(STATUS "Added local stb image from external/stb")

# =============================================================================
# FIND SYSTEM LIBRARIES
# =============================================================================
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# =============================================================================
# DEFINE THE EXECUTABLE
# =============================================================================
file(GLOB PROJECT_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# =============================================================================
# HANDLE SHADER ASSET DEPENDENCIES
# =============================================================================

file(GLOB SHADER_SOURCES "${CMAKE_SOURCE_DIR}/assets/shaders/*")

foreach(SHADER_SOURCE ${SHADER_SOURCES})
  get_filename_component(SHADER_FILENAME ${SHADER_SOURCE} NAME)
  set(SHADER_DEST "${CMAKE_BINARY_DIR}/shaders/${SHADER_FILENAME}")

  add_custom_command(
        OUTPUT ${SHADER_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_SOURCE} ${SHADER_DEST}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Copying shader ${SHADER_FILENAME}"
    )

  list(APPEND SHADER_DESTINATIONS ${SHADER_DEST})
endforeach()

add_custom_target(
    copy_shaders ALL
    DEPENDS ${SHADER_DESTINATIONS}
)

add_dependencies(${PROJECT_NAME} copy_shaders)



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
    stb
)

