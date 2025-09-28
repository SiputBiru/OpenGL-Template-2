OpenGL C++ Template with CMakeA simple and clean C++ template for getting started with modern OpenGL. This project uses CMake to handle the build process and manage dependencies like GLFW, GLM, and GLAD.Project StructureThe project is organized with a clear and scalable directory structure:.
├── assets/              # For shaders, textures, models, etc.
├── external/            # For third-party libraries (local or fetched by CMake).
├── include/             # For your project's header files.
├── src/                 # For your project's source files.
└── CMakeLists.txt       # The main build script for CMake.
DependenciesThis template includes the following essential libraries for OpenGL development:GLFW: Handles window creation, context, and user input. It is downloaded automatically by CMake using FetchContent.GLM (OpenGL Mathematics): A header-only library for vector and matrix math. It is also downloaded automatically by CMake.GLAD: Loads OpenGL function pointers. This template assumes you have placed the GLAD source files inside the external/glad directory.How to Build and RunBefore you begin, make sure you have a C++ compiler, CMake, and Ninja installed on your system.1. Configure the ProjectOpen a terminal in the root directory of the project. Run the following command to generate the build files. You only need to do this once, or whenever you modify CMakeLists.txt.cmake -S . -B build -G "Ninja"
-S . specifies that the source directory is the current one.-B build creates a build directory for all the build files.-G "Ninja" tells CMake to use the Ninja build system.2. Compile the CodeAfter the configuration is successful, compile the project by running this command from the root directory:cmake --build build
This will compile your source code and create the executable.3. Run the ExecutableThe final executable will be placed in the build directory. To run it, execute:./build/OpenGLProject
How to Add a New LibraryAll third-party libraries should be placed in the external/ directory. Here’s how to integrate them using CMake.Method 1: Fetching from a Git Repository (Recommended)If the library is available on a platform like GitHub, use CMake's FetchContent module.Declare the library in the DEPENDENCY MANAGEMENT section of your CMakeLists.txt:FetchContent_Declare(
    new_library   # A unique name for the dependency
    GIT_REPOSITORY [https://github.com/user/new-library.git](https://github.com/user/new-library.git)
    GIT_TAG main  # Or a specific version tag like v1.2.3
)
FetchContent_MakeAvailable(new_library)
Link the library to your executable in the CONFIGURE TARGET PROPERTIES section:target_link_libraries(${PROJECT_NAME} PRIVATE
    # ... other libraries
    new_library
)
Method 2: Adding a Local LibraryIf you have manually downloaded a library's source code into the external/ directory and it contains its own CMakeLists.txt, you can add it as a subdirectory.Place the library in external/. For example: external/my_local_lib.Add the subdirectory in the DEPENDENCY MANAGEMENT section of your CMakeLists.txt:add_subdirectory(${CMAKE_SOURCE_DIR}/external/my_local_lib)
Link the library to your executable. You will need to know the target name defined in the library's CMakeLists.txt.target_link_libraries(${PROJECT_NAME} PRIVATE
    # ... other libraries
    my_local_lib_target_name
)
