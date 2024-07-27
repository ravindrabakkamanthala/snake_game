cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Path to SFML
set(SFML_DIR "/usr/local/opt/sfml/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

include_directories(include)

add_executable(snake_game src/main.cpp)

target_link_libraries(snake_game sfml-system sfml-window sfml-graphics)