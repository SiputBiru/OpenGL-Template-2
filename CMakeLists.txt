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
# HANDLE ALL ASSET DEPENDENCIES (Shaders, Textures, Models, etc.)
# =============================================================================

# -- Define a reusable function to handle any asset subdirectory --
function(add_asset_directory TARGET_NAME ASSET_SUBDIR)
  file(GLOB_RECURSE ASSET_SOURCES "${CMAKE_SOURCE_DIR}/assets/${ASSET_SUBDIR}/*")

  set(ASSET_DESTINATIONS "") # Start with an empty list of destination files

  foreach(ASSET_SOURCE ${ASSET_SOURCES})
    file(RELATIVE_PATH RELATIVE_ASSET_PATH "${CMAKE_SOURCE_DIR}/assets/${ASSET_SUBDIR}" ${ASSET_SOURCE})

    set(ASSET_DEST "${CMAKE_BINARY_DIR}/${ASSET_SUBDIR}/${RELATIVE_ASSET_PATH}")

    add_custom_command(
            OUTPUT ${ASSET_DEST}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_SOURCE} ${ASSET_DEST}
            DEPENDS ${ASSET_SOURCE}
            COMMENT "Copying asset ${RELATIVE_ASSET_PATH}"
        )
    list(APPEND ASSET_DESTINATIONS ${ASSET_DEST})
  endforeach()

  add_custom_target(
        copy_${ASSET_SUBDIR} ALL
        DEPENDS ${ASSET_DESTINATIONS}
    )

  add_dependencies(${TARGET_NAME} copy_${ASSET_SUBDIR})
endfunction()


add_asset_directory(${PROJECT_NAME} shaders)
add_asset_directory(${PROJECT_NAME} textures)



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

