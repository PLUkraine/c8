cmake_minimum_required(VERSION 3.10.0)
project(c8 VERSION 0.1.0)

# Set C compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(MSVC)
  set(COMPILER_WARNINGS /W4 /WX)
else()
  set(COMPILER_WARNINGS -Wall -Wextra -Werror)
endif()

# profiler?
option(PROFILER_BUILD "compile for profiling" OFF)
set(PROFILER_FLAGS "")
if(${PROFILER_BUILD})
  message("Compiling for profiling")
  if(MSVC)
    set(PROFILER_FLAGS /PROFILE)
  else()
    set(PROFILER_FLAGS -pg)
  endif()
endif()


# find required libs
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# Testing?
include(CTest)
include(GoogleTest)
enable_testing()
add_subdirectory(tests)

# create targets
add_library(c8_lib STATIC
  src/logging.c
  src/c8.c
  src/c8_private.c
  src/c8_random.c
  src/c8_display.c
  src/c8_read.c)
target_compile_options(c8_lib PRIVATE ${COMPILER_WARNINGS} ${PROFILER_FLAGS})
target_include_directories(c8_lib PUBLIC src)

add_executable(c8
  src/main.c
  src/c8_app.c)
target_link_libraries(c8 PRIVATE SDL2::SDL2 c8_lib)
target_compile_options(c8 PRIVATE ${COMPILER_WARNINGS} ${PROFILER_FLAGS})

# CPack?
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
