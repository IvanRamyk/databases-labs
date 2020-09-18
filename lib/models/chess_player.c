//
// Created by Ivan Ramyk on 9/17/20.
//

#include <stdio.h>

#include "chess_player.h"

void print_chess_player_info(chess_player player) {
    printf("Chess player no %d: %s, %d years old, rating: %d\n",
            player.id, player.full_name, player.age, player.rating);
}