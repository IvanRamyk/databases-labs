//
// Created by Ivan Ramyk on 9/17/20.
//

#ifndef LAB1_CHESS_PLAYER_H
#define LAB1_CHESS_PLAYER_H

typedef struct ChessPlayer {
    int id;
    char full_name[45];
    int age;
    int rating;
} chess_player;

void print_chess_player_info(chess_player player);

#endif //LAB1_CHESS_PLAYER_H
