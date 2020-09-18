cmake_minimum_required(VERSION 3.14)
project(lab1 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab1 main.c lib/models/chess_player.c lib/models/chess_player.h lib/models/chess_club_member.c lib/models/chess_club_member.h lib/database/database.c lib/database/database.h lib/algorithms/sort.c lib/algorithms/sort.h lib/database_interface/database_console_interface.h)