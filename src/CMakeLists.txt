Cmake_minimum_required(VERSION 2.6)

project(Utictactoe C)

add_executable(utictactoe utictactoe.c model.c view.c ia.c)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g -I../include")

set(CMAKE_C_COMPILER gcc)

set(CMAKE_INSTALL_PREFIX ".")

include_directories("../include")

install(TARGETS utictactoe DESTINATION bin)

target_link_libraries(utictactoe ncurses)