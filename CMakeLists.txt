cmake_minimum_required(VERSION 3.25)
project(rpg_demo_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pull rpgkit straight from GitHub, pinned to a release tag. This is the
# whole dependency story — no install step, no package manager.
include(FetchContent)
FetchContent_Declare(
  rpgkit
  GIT_REPOSITORY https://github.com/KirkDiggler/rpgkit.git
  GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(rpgkit)

add_executable(demo_game src/main.cpp)
target_link_libraries(demo_game PRIVATE rpg::core)
