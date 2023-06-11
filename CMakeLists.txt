cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(block_game)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG ac13ca9
)
FetchContent_MakeAvailable(SDL2)
set(SDL2_MAIN_LIBRARY ON)
add_executable(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -std=c++20)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL2_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)
target_sources(${PROJECT_NAME} PRIVATE 
 src/main.cpp
 src/entity.cpp
)
