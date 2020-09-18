//
// Created by Ivan Ramyk on 9/17/20.
//

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void update_member(chess_database database, chess_club_member member) {

}

void insert_player(chess_database database, chess_player player) {

}

void insert_member(chess_database database, chess_club_member member) {

}

chess_player get_player(chess_database database, int player_id) {
    chess_player result;
    return result;
}

chess_club_member get_member(chess_database database, int player_id, int member_id) {
    chess_club_member result;
    return result;
}

void del_player(chess_database database, int player_id) {

}

void del_member(chess_database database, int player_id, int member_id) {

}

void update_player(chess_database database, chess_player player) {

}

int get_cnt_players(chess_database database) {
    return 0;
}

int get_cnt_members(chess_database database) {
    return 0;
}

int get_cnt_clubs(chess_database database, int player_id) {
    return 0;
}

void utl_players(chess_database database) {

}

void utl_index_players(chess_database database) {
    FILE* player_index_file_pointer = fopen(database.player_index_path, "rb");

    int cnt_players;
    fread(&cnt_players, sizeof(int), 1, player_index_file_pointer);
    int table[2*cnt_players];
    if (cnt_players != 0)
        fread(table, sizeof(int), cnt_players*2, player_index_file_pointer + sizeof(int));
    fclose(player_index_file_pointer);

    printf("%d\n", cnt_players);
    for (int i = 0; i < cnt_players; ++i){
        printf("%d %d\n", table[i * 2], table[i * 2 + 1]);
    }
}

void utl_members(chess_database database) {

}

void read_index_file(chess_database database) {
    printf("reading...\n");
    FILE* player_index_file_pointer = fopen(database.player_index_path, "rb");
    fread(&database.cnt_players, sizeof(int), 1, player_index_file_pointer);
    database.ind_table = malloc(2 * database.cnt_players * sizeof(int));
    if (database.cnt_players != 0) {
        fread(&database.ind_table, sizeof(int), database.cnt_players * 2, player_index_file_pointer + sizeof(int));
    }
    fclose(player_index_file_pointer);
}

void create_index_file(chess_database database) {
    printf("creating...\n");
    FILE* player_index_file_pointer = fopen(database.player_index_path, "wb");
    int cnt_players = 0;
    fwrite(&cnt_players, sizeof(int), 1, player_index_file_pointer);
    fclose(player_index_file_pointer);
}

void init_database(chess_database database) {
    if( access(database.player_index_path, F_OK ) == -1 ) {
        create_index_file(database);
    }
    read_index_file(database);
}
