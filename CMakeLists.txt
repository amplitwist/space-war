cmake_minimum_required(VERSION 3.20)

project(space-war LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR SDL2/cmake)
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/game.cpp
  src/actor.cpp
  src/component.cpp
  src/star.cpp
  src/sprite_component.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
  SDL2::SDL2main
)
