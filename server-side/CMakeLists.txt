cmake_minimum_required(VERSION 3.18.4)
project(server_side C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SFML 2.5.1 COMPONENTS system network REQUIRED)

add_executable(server_side main.c
        base/movement.h
        base/network.h
        player/player.c
        player/player.h
        application/application.c
        application/application.h
        ../common/linked_list/linked_list.c
        ../common/linked_list/linked_list.h
        application/application.c
        server_controller/server_controller.c
        server_controller/server_controller.h
        ../common/base/server_message_types.h
)

target_link_libraries(server_side csfml-system csfml-network)
