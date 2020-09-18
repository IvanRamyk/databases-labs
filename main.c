#include <stdio.h>
#include "lib/models/chess_player.h"
#include "lib/models/chess_club_member.h"
#include "lib/database/database.h"

int main() {
    chess_database database = {"/Users/ivanramyk/dev/databases/lab1/player.ind",
                               "/Users/ivanramyk/dev/databases/lab1/player.fl",
                               "/Users/ivanramyk/dev/databases/lab1member.fl"};
    init_database(database);
    utl_index_players(database);
    return 0;
}