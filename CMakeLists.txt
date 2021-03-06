cmake_minimum_required(VERSION 3.10.0)
project(c8 VERSION 1.0.0)


# options
option(USE_CONAN "configure with conan" OFF)
option(PRINT_OPCODES "print dissassembly info" OFF)
option(PROFILER_BUILD "compile for profiling" OFF)


# dependencies
if (${USE_CONAN})
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
else()
  find_package(SDL2 REQUIRED)
  find_package(GTest REQUIRED)
endif()


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


# profiler (gcc or clang)
set(PROFILER_FLAGS "")
if(NOT MSVC AND ${PROFILER_BUILD})
  message("Compiling for profiling")
  set(PROFILER_FLAGS -pg)
endif()


# testing
include(CTest)
include(GoogleTest)
enable_testing()
add_subdirectory(tests)


# create library target
add_library(c8_lib STATIC
  src/logging.c
  src/c8.c
  src/c8_private.c
  src/c8_random.c
  src/c8_display.c
  src/c8_read.c
  src/c8_keyboard.c)
if (${PRINT_OPCODES})
  message("Defining LOG_OPCODE...")
  target_compile_definitions(c8_lib PRIVATE C8_LOG_OPCODE=1)
endif()
target_compile_options(c8_lib PRIVATE ${COMPILER_WARNINGS} ${PROFILER_FLAGS})
target_include_directories(c8_lib PUBLIC src)


# create app target
add_executable(c8
  src/main.c
  src/c8_app.c)
target_compile_options(c8 PRIVATE ${COMPILER_WARNINGS} ${PROFILER_FLAGS})

if (${USE_CONAN})
  target_link_libraries(c8 PRIVATE CONAN_PKG::sdl2 c8_lib)
else()
  target_link_libraries(c8 PRIVATE SDL2::SDL2 c8_lib)
endif()


# profiler (visual studio)
if(MSVC AND ${PROFILER_BUILD})
  message("Compiling for profiling")
  SET_TARGET_PROPERTIES(c8_lib PROPERTIES LINK_FLAGS "/PROFILE")
  SET_TARGET_PROPERTIES(c8 PROPERTIES LINK_FLAGS "/PROFILE")
endif()
