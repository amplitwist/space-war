cmake_minimum_required(VERSION 3.20)

project(space-war LANGUAGES CXX)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/game.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
)
