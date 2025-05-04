cmake_minimum_required(VERSION 3.29)
project(ECEARENA C)

set(CMAKE_C_STANDARD 23)

add_executable(ECEARENA main.c
        graphics.c
        ui.c
        game.c
        init.c)

target_link_libraries(ECEARENA -lmingw32 -lalleg44)
