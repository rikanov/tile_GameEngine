cmake_minimum_required(VERSION 2.6)
project(engine)

add_executable(game_engine main.cpp engine.cpp tile.cpp board.cpp move.cpp field.cpp node.cpp )
target_link_libraries(game_engine ${CMAKE_SOURCE_DIR}/../View2D/build/libview2d.a ${CMAKE_SOURCE_DIR}/../BoardView/build/libboardview.a)

install(TARGETS game_engine RUNTIME DESTINATION bin)
