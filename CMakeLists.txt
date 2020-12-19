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

# find required libs
find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# Testing?
include(CTest)
include(GoogleTest)
enable_testing()
add_subdirectory(tests)

# create executable
add_executable(${PROJECT_NAME} src/main.c src/cummon.h src/logging.h src/logging.c)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_WARNINGS})

# CPack?
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
