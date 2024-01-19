cmake_minimum_required(VERSION 3.20)

project(space-war LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR SDL2/cmake)
find_package(SDL2 REQUIRED)
set(SDL2_image_DIR SDL2_image/cmake)
find_package(SDL2_image REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/game.cpp
  src/actor.cpp
  src/component.cpp
  src/star.cpp
  src/sprite_component.cpp
  src/camera.cpp
  src/planet.cpp
  src/orbit_component.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
  SDL2::SDL2main
  SDL2_image::SDL2_image
)
