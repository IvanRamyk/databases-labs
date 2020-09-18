//
// Created by Ivan Ramyk on 9/18/20.
//

#ifndef LAB1_DATABASE_CONSOLE_INTERFACE_H
#define LAB1_DATABASE_CONSOLE_INTERFACE_H

#include <stdio.h>
#include "../database/database.h"
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
            int id, age, rating;
            char full_name[45];
            scanf("%d\n%s\n%d\n%d", &id, full_name, &age, &rating);
            chess_player player = {id, "fdsfsd", age, rating};
            print_chess_player_info(player);
            insert_player(&console_interface->database, player);
        }
        if (strcmp(command, "print_players") == 0) {
            utl_players(&console_interface->database);
        }
        if (strcmp(command, "get_player") == 0) {
            int id;
            scanf("%d", &id);
            chess_player result = get_player(&console_interface->database, id);
            print_chess_player_info(result);
        }
        printf("OK\n");
    }
}

#endif //LAB1_DATABASE_CONSOLE_INTERFACE_H
