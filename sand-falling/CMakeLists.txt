cmake_minimum_required(VERSION 3.15)

project(SandFalling)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_NCURSES_LIBRARY})
