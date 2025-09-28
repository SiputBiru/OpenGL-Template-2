OpenGL CMake TemplateA simple, cross-platform C++ template for getting started with OpenGL projects. This template uses CMake for building and manages dependencies like GLFW, GLAD, and GLM.The project is structured to be clean and easy to expand upon. Dependencies are handled automatically by CMake or are included locally.Directory StructureThe project follows a standard directory layout:.
├── assets/              # Shaders, textures, models, etc.
├── external/            # Third-party libraries (managed by CMake or manually)
├── include/             # Project-specific header files
├── src/                 # Project-specific source files
└── CMakeLists.txt       # The main build script for CMake
DependenciesThis template uses the following core libraries:GLFW: For creating windows and handling input. (Managed by FetchContent)GLM: For OpenGL mathematics. (Managed by FetchContent)GLAD: For loading OpenGL function pointers. (Included locally in external/)GLFW and GLM are downloaded automatically by CMake during the configuration step. You only need to have the necessary system libraries for OpenGL development installed.How to Build and RunYou'll need a modern C++ compiler, CMake, and Ninja installed on your system.1. Configure the ProjectFirst, create a build directory and run CMake to generate the build files. This command only needs to be run once, or whenever you change the CMakeLists.txt.Open your terminal in the root directory of the project and run:cmake -S . -B build -G "Ninja"
-S . tells CMake that the source directory is the current directory.-B build tells CMake to place the build files in a new directory named build.-G "Ninja" tells CMake to generate build files for the Ninja build system.2. Compile the CodeOnce configuration is complete, you can compile the project.Run this command from the root directory:cmake --build build
Alternatively, you can navigate into the build directory and run ninja directly.3. Run the ExecutableThe compiled executable will be located inside the build directory../build/OpenGLProject
How to Add a LibraryAdding new libraries is straightforward. The recommended approach is to place the library's source code inside the external directory and modify the main CMakeLists.txt.Example: Adding a library using FetchContentThis is the preferred method if the library is available on a git repository.Declare the content: Add the following lines to the DEPENDENCY MANAGEMENT section in CMakeLists.txt, similar to how glfw and glm are handled.# In CMakeLists.txt
FetchContent_Declare(
    new_library_name
    GIT_REPOSITORY [https://github.com/user/new-library.git](https://github.com/user/new-library.git)
    GIT_TAG main # Or a specific version/commit
)
FetchContent_MakeAvailable(new_library_name)
Link the library: Add the library's target name to your executable in the CONFIGURE TARGET PROPERTIES section.# In CMakeLists.txt
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glad
    new_library_name # Add the new library here
)
Example: Adding a local library using add_subdirectoryUse this method if you have downloaded the library's source code manually into the external folder.Add the subdirectory: If the library has its own CMakeLists.txt, you can add it to the build with one line in the DEPENDENCY MANAGEMENT section.# In CMakeLists.txt, assuming you placed it in external/my_local_lib
add_subdirectory(${CMAKE_SOURCE_DIR}/external/my_local_lib)
Link the library: Find the target name defined by the library's CMake file and add it to target_link_libraries just like in the example above.
