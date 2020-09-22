//
// Created by Ivan Ramyk on 9/18/20.
//

#ifndef LAB1_DATABASE_CONSOLE_INTERFACE_H
#define LAB1_DATABASE_CONSOLE_INTERFACE_H

#include <stdio.h>
#include "database.h"
#include <string.h>

struct DatabaseConsoleInterface {
    chess_database database;
} typedef database_console_interface;

void run_interface(database_console_interface* console_interface) {
    while (1) {
        char command[20];
        scanf("%s", command);

        if (strcmp(command, "exit") == 0)
            break;

        if (strcmp(command, "insert_player") == 0) {
            chess_player player;
            scanf("%d\n%s\n%d\n%d", &player.id, player.full_name, &player.age, &player.rating);
            insert_player(&console_interface->database, player);
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "print_players") == 0) {
            utl_players(&console_interface->database);
            continue;
        }

        if (strcmp(command, "get_player") == 0) {
            int id;
            scanf("%d", &id);
            chess_player result = get_player(&console_interface->database, id);
            print_chess_player_info(result);
            continue;
        }

        if (strcmp(command, "get_cnt_of_player_memberships") == 0) {
            int id;
            scanf("%d", &id);
            int result = get_cnt_player_members(&console_interface->database, id);
            printf("%d\n", result);
            continue;
        }

        if (strcmp(command, "get_cnt_members") == 0) {
            int result = get_cnt_members(&console_interface->database);
            printf("%d\n", result);
            continue;
        }

        if (strcmp(command, "get_cnt_players") == 0) {
            int result = get_cnt_players(&console_interface->database);
            printf("%d\n", result);
            continue;
        }

        if (strcmp(command, "print_members") == 0) {
            utl_members(&console_interface->database);
            continue;
        }

        if (strcmp(command, "insert_member") == 0) {
            chess_club_member member;
            scanf("%d\n%d\n%s\n%s", &member.id, &member.player_id, member.club_name, member.position);
            if (get_player(&console_interface->database, member.player_id).id != -1) {
                insert_member(&console_interface->database, member);
            }
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "get_member") == 0) {
            int id, player_id;
            scanf("%d %d", &player_id, &id);
            chess_club_member result = get_member(&console_interface->database, player_id, id);
            print_chess_club_member_info(result);
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "update_player") == 0) {
            chess_player player;
            scanf("%d\n%s\n%d\n%d", &player.id, player.full_name, &player.age, &player.rating);
            update_player(&console_interface->database, player);
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "update_member") == 0) {
            chess_club_member member;
            scanf("%d\n%d\n%s\n%s", &member.id, &member.player_id, member.club_name, member.position);
            int res = update_member(&console_interface->database, member);
            printf("%d\n", res);
            continue;
        }

        if (strcmp(command, "delete_member") == 0) {
            int id, player_id;
            scanf("%d %d", &id, &player_id);
            del_member(&console_interface->database, player_id, id);
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "delete_player") == 0) {
            int id;
            scanf("%d", &id);
            del_player(&console_interface->database, id);
            printf("OK\n");
            continue;
        }

        if (strcmp(command, "print_index") == 0) {
            utl_index_players(&console_interface->database);
            continue;
        }
        printf("invalid command\n");
    }
}

#endif //LAB1_DATABASE_CONSOLE_INTERFACE_H
