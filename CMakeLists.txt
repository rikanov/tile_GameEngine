cmake_minimum_required(VERSION 2.6)
project(gameengine)

add_executable(gameengine main.cpp)

install(TARGETS gameengine RUNTIME DESTINATION bin)
