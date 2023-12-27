cmake_minimum_required(VERSION 3.18.4)
project(server_side C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SFML 2.5.1 COMPONENTS system network REQUIRED)

add_executable(server_side main.c
        base/movement.h
        base/network.h
        player/player.c
        player/player.h)

target_link_libraries(server_side csfml-system csfml-network)
