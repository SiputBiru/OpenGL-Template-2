# 🚀 OpenGL C++ CMake Template

A modern, cross-platform C++ template for starting OpenGL projects. It uses a clean project structure and manages dependencies with CMake's FetchContent module.

**C++17** | **CMake** | **OpenGL** | **GLFW** | **GLM** | **GLAD**

---

## ✨ Features

* **Modern CMake:** Clean and easy-to-understand `CMakeLists.txt` using modern CMake practices.
* **Automatic Dependency Fetching:** GLFW and GLM are downloaded automatically during the build configuration.
* **Cross-Platform:** Designed to work on Windows, macOS, and Linux.
* **Simple Structure:** A clear and scalable directory layout for source files, headers, and assets.
* **Ready to Use:** Includes pre-configured GLAD for loading OpenGL functions.

---

## 📂 Project Structure

The project is organized to keep code, assets, and third-party libraries separate and clean.
.
├── assets/              # Shaders, textures, models, etc.
├── external/            # Third-party libraries (local or fetched by CMake)
├── include/             # Your project's header files (.h, .hpp)
├── src/                 # Your project's source files (.cpp)
└── CMakeLists.txt       # The main build script for CMake


---

## 🛠️ Getting Started

Follow these steps to configure, build, and run the project.

### Prerequisites

Before you begin, ensure you have the following software installed:
* A C++17 compliant compiler (GCC, Clang, MSVC)
* [CMake](https://cmake.org/download/) (version 3.14 or higher)
* [Ninja](https://ninja-build.org/) (or another build system like Make)

### Build and Run Instructions

1.  **Configure the Project**

    Open a terminal in the project's root directory and run the following command. This will generate the build files inside a `build/` directory.

    ```bash
    cmake -S . -B build -G "Ninja"
    ```
    > **Note:** You only need to run this configuration command once, or whenever you make changes to `CMakeLists.txt`.

2.  **Compile the Code**

    Now, build the project using CMake's build command. This compiles your code and creates the executable.

    ```bash
    cmake --build build
    ```

3.  **Run the Application**

    The final executable will be located in the `build/` directory. You can run it directly from your terminal:

    ```bash
    ./build/OpenGLProject
    ```

---

## 📦 How to Add a New Library

The `external/` directory is the designated place for all third-party libraries. Here’s how to integrate them into your build.

### Method 1: Fetching from a Git Repository (Recommended)

This is the best approach for libraries available on platforms like GitHub.

1.  **Declare the library** in the `DEPENDENCY MANAGEMENT` section of your `CMakeLists.txt`.
    ```cmake
    FetchContent_Declare(
        new_library_name   # A unique name for this dependency
        GIT_REPOSITORY [https://github.com/user/new-library.git](https://github.com/user/new-library.git)
        GIT_TAG main       # Or a specific version tag like v1.2.3
    )
    FetchContent_MakeAvailable(new_library_name)
    ```

2.  **Link the library** to your executable in the `CONFIGURE TARGET PROPERTIES` section.
    ```cmake
    target_link_libraries(${PROJECT_NAME} PRIVATE
        # ... other libraries
        new_library_name # Add the new library's target name here
    )
    ```

### Method 2: Using a Local Library Subdirectory

Use this method if you have manually downloaded a library's source code and it contains its own `CMakeLists.txt`.

1.  **Place the library** in `external/`. For example: `external/my_local_lib`.

2.  **Add the subdirectory** in the `DEPENDENCY MANAGEMENT` section of `CMakeLists.txt`.
    ```cmake
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/my_local_lib)
    ```

3.  **Link the library** to your executable. You will need to find the target name defined by that library's CMake file.
    ```cmake
    target_link_libraries(${PROJECT_NAME} PRIVATE
        # ... other libraries
        my_local_lib_target_name
    )
    ```

---
