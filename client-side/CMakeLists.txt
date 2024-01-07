cmake_minimum_required(VERSION 3.18.4)
project(client_side C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SFML 2.5.1 COMPONENTS system network REQUIRED)

add_executable(client_side main.c
        application/application.c
        application/application.h
        bullet/bullet.c
        bullet/bullet.h
        map/map.c
        map/map.h
        tank/tank.c
        tank/tank.h
        windows/main_menu/main_menu.c
        windows/main_menu/main_menu.h
        windows/menu/menu.c
        windows/menu/menu.h
        windows/show_score/show_score.c
        windows/show_score/show_score.h
        windows/show_status/show_status.c
        windows/show_status/show_status.h
        windows/elements/button.c
        windows/elements/button.h
        windows/elements/textbox.c
        windows/elements/textbox.h
        ../common/linked_list/linked_list.c
        ../common/linked_list/linked_list.h
        ../common/base/server_message_types.h
        ../common/base/type_of_maps.h
        server_controller/server_controller.c
        server_controller/server_controller.h
)

target_link_libraries(client_side csfml-system csfml-network csfml-graphics)
