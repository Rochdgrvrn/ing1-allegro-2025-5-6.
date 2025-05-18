cmake_minimum_required(VERSION 3.14)
project(ecearena2 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(ECEARENA2 ${SOURCES}
        main.c
        src/globals.c
        src/spell.c
        include/spell.h
        src/assets.c
        include/assets.h
        include/images.h)

target_link_libraries(ECEARENA2 mingw32 alleg44)
