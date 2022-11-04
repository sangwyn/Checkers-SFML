cmake_minimum_required(VERSION 3.23)
project(game_with_ai)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(game_with_ai main.cpp Bot.cpp Bot.h EasyBot.cpp EasyBot.h MediumBot.cpp MediumBot.h HardBot.cpp HardBot.h Globals.h GameManager.cpp GameManager.h SearchTreeNode.cpp SearchTreeNode.h SearchTree.cpp SearchTree.h Button.cpp Button.h)
target_link_libraries(game_with_ai sfml-graphics sfml-audio)
