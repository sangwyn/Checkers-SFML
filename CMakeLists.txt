cmake_minimum_required(VERSION 3.23)
project(game_with_ai)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(game_with_ai main.cpp)
target_link_libraries(game_with_ai sfml-graphics sfml-audio)
