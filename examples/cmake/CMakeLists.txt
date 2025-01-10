# Sets DEVKIT_PRO variable to directory with devkitPro
set(DEVKIT_PRO "/opt/devkitpro")

# Include needed to build with Wii toolchain
include("${DEVKIT_PRO}/cmake/Wii.cmake")
# Includes needed to find libraries needed by GRRLIB
include(FindZLIB)
include(FindJPEG)
include(FindPNG)

cmake_minimum_required(VERSION 3.5)

# Your project details
project(
    cmake # Your project name goes here
    VERSION 0.1.0 # Your project version goes here
    LANGUAGES C # Or CXX if you using C++
)

# Libraries required to link GRRLIB
find_package(Freetype REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

# Finds some required libraries
file(GLOB libraries
    "${DEVKIT_PRO}/portlibs/wii/lib/*.a" # GRRLIB
    "${DEVKIT_PRO}/libogc/lib/wii/*.a" # libfat
)

# GRRLIB includes
include_directories("${DEVKIT_PRO}/portlibs/wii/include")
# Links the whole of libraries
link_libraries(${libraries} ${FREETYPE_LIBRARIES} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} -lfat)

# Here we go, builds executable
add_executable(cmake "src/main.c")
