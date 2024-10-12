cmake_minimum_required(VERSION 3.18)
project(grrlib)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(FREETYPE REQUIRED freetype2)
pkg_check_modules(JPEG REQUIRED libjpeg)

add_library(pngu STATIC)

target_sources(pngu
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/lib/pngu/pngu.c"
)

target_include_directories(pngu
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/lib/pngu>"
  PRIVATE
  ${PNG_INCLUDE_DIRS}
)

target_link_libraries(pngu PUBLIC
  ${PNG_LIBRARIES}
)

add_library(grrlib STATIC)

target_compile_options(grrlib
  PRIVATE
  -Wall
  -Wshadow
  -Wunused
)

file(GLOB_RECURSE GRRLIB_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/GRRLIB/*.c")
target_sources(grrlib
  PRIVATE
  "${GRRLIB_SRC_FILES}"
)

target_include_directories(grrlib
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/GRRLIB>"
  PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/GRRLIB/grrlib>"
  ${FREETYPE_INCLUDE_DIRS}
  ${JPEG_INCLUDE_DIRS}
)

target_link_libraries(grrlib PUBLIC
  pngu
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/GRRLIB/GRRLIB/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)

install(TARGETS pngu grrlib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
