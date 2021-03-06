//
// Created by Ivan Ramyk on 9/17/20.
//

#ifndef LAB1_DATABASE_H
#define LAB1_DATABASE_H

#include "../models/chess_player.h"
#include "../models/chess_club_member.h"
#include "../models/chess_club_member.h"

typedef struct ChessDatabase {
    // required fields
    char * player_index_path;
    char * player_file_path;
    char * member_file_path;

    // fields managed by base
    int cnt_players;
    int ind_table[200];
} chess_database;

void init_database(chess_database* database);

void insert_player(chess_database* database, chess_player player); //insert-m

void insert_member(chess_database* database, chess_club_member member); //insert-s

chess_player get_player(chess_database* database, int player_id); //get-m

chess_club_member get_member(chess_database* database, int player_id, int member_id); //get-s

void del_player(chess_database* database, int player_id); //del-m

void del_member(chess_database* database, int player_id, int member_id); //del-s

int update_player(chess_database* database, chess_player player); //update-m

int update_member(chess_database* database, chess_club_member member); //update-s

int get_cnt_players(chess_database* database);

int get_cnt_members(chess_database* database);

int get_cnt_player_members(chess_database* database, int player_id);

void utl_players(chess_database* database);

void utl_members(chess_database* database);

void utl_index_players(chess_database* database);

void print_players(chess_database* database);

void print_members(chess_database* database);


#endif //LAB1_DATABASE_H
