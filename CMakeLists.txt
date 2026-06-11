cmake_minimum_required(VERSION 3.25)
project(rpgkit_demo_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  rpgkit
  GIT_REPOSITORY https://github.com/KirkDiggler/rpgkit.git
  GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(rpgkit)

add_executable(rpgkit_demo_game src/main.cpp)
target_link_libraries(rpgkit_demo_game PRIVATE rpg::core)
