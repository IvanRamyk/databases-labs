//
// Created by Ivan Ramyk on 9/17/20.
//

#include <stdio.h>

#include "chess_club_member.h"

void print_chess_club_member_info(chess_club_member member) {
    printf("Chess member no %d: club: %s, player no: %d, position: %s\n",
           member.id, member.club_name, member.player_id, member.position);
}