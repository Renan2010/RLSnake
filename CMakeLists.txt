cmake_minimum_required(VERSION 3.10)
project(RLsnake)
add_executable(RLsnake src/main.cpp)
target_link_libraries(RLsnake sfml-graphics sfml-window sfml-system)
