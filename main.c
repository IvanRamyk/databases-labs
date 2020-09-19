#include <stdio.h>
#include "lib/models/chess_player.h"
#include "lib/models/chess_club_member.h"
#include "lib/database/database.h"
#include "lib/database_interface/database_console_interface.h"

int main() {
    chess_database database = {"/Users/ivanramyk/dev/databases/lab1/player.ind",
                               "/Users/ivanramyk/dev/databases/lab1/player.fl",
                               "/Users/ivanramyk/dev/databases/lab1/member.fl"};
    init_database(&database);
    database_console_interface console_interface = {database};
    run_interface(&console_interface);

//    chess_player player1 = {1, "Ivan", 21, 12};
//    insert_player(&database, player1);
//    insert_player(&database, player1);
//    insert_player(&database, player1);
//
//
//    chess_player player2 = {2, "Sergey", 22, 33};
//    insert_player(&database, player2);
//    insert_player(&database, player2);
//    insert_player(&database, player2);
//
//    utl_players(&database);
//
//    print_chess_player_info(get_player(&database, 1));
//    print_chess_player_info(get_player(&database, 2));
    return 0;
}