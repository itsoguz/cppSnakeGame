cmake_minimum_required(VERSION 3.10)

# Set CMake policy for OpenGL preference to NEW (GLVND)
# This suppresses the CMP0072 warning and ensures modern OpenGL libraries are preferred.
cmake_policy(SET CMP0072 NEW)

# Explicitly tell FindOpenGL to prefer GLVND libraries.
set(OpenGL_GL_PREFERENCE "GLVND")

project(qtSnake VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

# Specify where to place the executable binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find Qt6 packages
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)

# Add the source subdirectory
add_subdirectory(src)

