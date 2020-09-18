//
// Created by Ivan Ramyk on 9/17/20.
//

#ifndef LAB1_CHESS_CLUB_MEMBER_H
#define LAB1_CHESS_CLUB_MEMBER_H

typedef struct ChessClubMember {
    int id;
    int player_id;
    char club_name[45];
    char position[45];
} chess_club_member;

void print_chess_club_member_info(chess_club_member member);

#endif //LAB1_CHESS_CLUB_MEMBER_H
