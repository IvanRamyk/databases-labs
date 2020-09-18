//
// Created by Ivan Ramyk on 9/17/20.
//

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void print_ind_table(chess_database* database) {
    printf("%d\n", database->cnt_players);
    for (int i = 0; i < database->cnt_players * 2; ++i) {
        printf("%d ",database->ind_table[i]);
    }
    printf("\n");
}

void update_index_file(chess_database* database) {
    print_ind_table(database);
    FILE* player_index_file_pointer = fopen(database->player_index_path, "wb");
    fwrite(&database->cnt_players, sizeof(int), 1, player_index_file_pointer);
    if (database->cnt_players != 0) {
        //fseek(player_index_file_pointer, sizeof(int), SEEK_SET);
        fwrite(database->ind_table, sizeof(int), database->cnt_players * 2,
               player_index_file_pointer);
    }
    fclose(player_index_file_pointer);
}

void insert_player_to_index_table(chess_database* database, chess_player player) {
    database->cnt_players++;
    database->ind_table[(database->cnt_players - 1) * 2] = player.id;
    database->ind_table[(database->cnt_players - 1) * 2 + 1] = database->cnt_players -1;
    update_index_file(database);
}

void insert_player_to_file(chess_database* database, chess_player player) {
    FILE* player_file_pointer = fopen(database->player_file_path, "ab");
    unsigned size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * (database->cnt_players - 1);
    fseek(player_file_pointer, offset, SEEK_SET);
    fwrite(&player, sizeof(chess_player), 1, player_file_pointer);
    int additional_info[2] = {-1, 0}; // position of first s, and cnt of sons
    fwrite(additional_info, sizeof(int), 2, player_file_pointer);
    fclose(player_file_pointer);
}

void update_member(chess_database* database, chess_club_member member) {

}

void insert_player(chess_database* database, chess_player player) {
    insert_player_to_index_table(database, player);
    insert_player_to_file(database, player);
}

void insert_member(chess_database* database, chess_club_member member) {

}

chess_player get_player_by_row(chess_database* database, int row) {
    FILE* player_file_pointer = fopen(database->player_file_path, "rb");
    unsigned size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * row;
    fseek(player_file_pointer, offset, SEEK_SET);
    chess_player player;
    fread(&player, sizeof(chess_player), 1, player_file_pointer);
    fclose(player_file_pointer);
    return player;
}

chess_player get_player(chess_database* database, int player_id) {
    for (int i = 0; i < database->cnt_players; ++i) {
        if (database->ind_table[i * 2] == player_id) {
            int row = database->ind_table[i * 2 + 1];
            return get_player_by_row(database, row);
        }
    }
    chess_player player = {-1, "", 0, 0};
    return player;
}

chess_club_member get_member(chess_database* database, int player_id, int member_id) {
    chess_club_member result;
    return result;
}

void del_player(chess_database* database, int player_id) {

}

void del_member(chess_database* database, int player_id, int member_id) {

}

void update_player(chess_database* database, chess_player player) {

}

int get_cnt_players(chess_database* database) {
    return 0;
}

int get_cnt_members(chess_database* database) {
    return 0;
}

int get_cnt_clubs(chess_database* database, int player_id) {
    return 0;
}

void utl_players(chess_database* database) {
    FILE* player_file_pointer = fopen(database->player_file_path, "rb");
    int cnt_players = database->cnt_players;
    for (int i = 0; i < cnt_players; ++i) {
        chess_player cur;
        int son_position;
        int cnt_sons;
        fread(&cur, sizeof(chess_player), 1, player_file_pointer);
        fread(&son_position, sizeof(int), 1, player_file_pointer);
        fread(&cnt_sons, sizeof(int), 1, player_file_pointer);
        printf("%d %s %d %d %d %d\n",
               cur.id, cur.full_name, cur.age, cur.rating, son_position, cnt_sons);
    }
    fclose(player_file_pointer);
}

void utl_index_players(chess_database* database) {
    FILE* player_index_file_pointer = fopen(database->player_index_path, "rb");

    int cnt_players;
    fread(&cnt_players, sizeof(int), 1, player_index_file_pointer);
    int table[2*cnt_players];
    if (cnt_players != 0) {
        fread(table, sizeof(int), cnt_players * 2, player_index_file_pointer);
    }
    for (int i = 0; i < 2*cnt_players;++i)
        printf("%d ", table[i]);
    printf("\n");
    fclose(player_index_file_pointer);

}

void utl_members(chess_database* database) {

}

void read_index_file(chess_database* database) {
    printf("reading...\n");
    FILE* player_index_file_pointer = fopen(database->player_index_path, "rb");
    fread(&database->cnt_players, sizeof(int), 1, player_index_file_pointer);
    database->table_size = database->cnt_players;
    if (database->cnt_players != 0) {
        fread(&database->ind_table, sizeof(int), database->cnt_players * 2, player_index_file_pointer);
    }
    fclose(player_index_file_pointer);
}

void create_index_file(chess_database* database) {
    printf("creating...\n");
    FILE* player_index_file_pointer = fopen(database->player_index_path, "wb");
    int cnt_players = 0;
    fwrite(&cnt_players, sizeof(int), 1, player_index_file_pointer);
    fclose(player_index_file_pointer);
}

void init_database(chess_database* database) {
    FILE* player_index_file_pointer = fopen(database->player_file_path, "wb");
    fclose(player_index_file_pointer);
    if( access(database->player_index_path, F_OK ) == -1 ) {
        create_index_file(database);
    }
    read_index_file(database);
}
