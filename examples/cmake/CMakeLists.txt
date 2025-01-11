cmake_minimum_required(VERSION 3.18)

project(
  application # Your project name goes here
  VERSION 1.0.0 # Your project version goes here
  LANGUAGES C # Or CXX if you using C++
)

# Libraries required to link GRRLIB
find_library(GRRLIB grrlib REQUIRED)
find_library(PNGU pngu REQUIRED)
find_library(FAT fat
  PATHS "${OGC_ROOT}/lib/${OGC_SUBDIR}"
  REQUIRED
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JPEG REQUIRED libjpeg)
pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(FREETYPE REQUIRED freetype2)

add_executable(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${DEVKITPRO}/portlibs/${OGC_CONSOLE}/include
)

target_sources(${PROJECT_NAME} PRIVATE
  "src/main.c"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${GRRLIB}
  ${PNGU}
  ${PNG_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${FAT}
)

# Here we go, builds executable
ogc_create_dol(${PROJECT_NAME})
