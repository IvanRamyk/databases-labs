//
// Created by Ivan Ramyk on 9/17/20.
//

#include "database.h"
#include <stdio.h>
#include <unistd.h>


void update_index_file(chess_database* database) {
    printf("%d!\n", database->cnt_players);
    FILE* player_index_file_pointer = fopen(database->player_index_path, "wb");
    fwrite(&database->cnt_players, sizeof(int), 1, player_index_file_pointer);
    if (database->cnt_players != 0) {
        fwrite(database->ind_table, sizeof(int), database->cnt_players * 2,
               player_index_file_pointer);
    }
    fclose(player_index_file_pointer);
}

void insert_player_to_index_table(chess_database* database, chess_player player) {
    database->cnt_players++;
    for (int i = database->cnt_players - 1; i >= 0; i--) {
        if (i == 0 || database->ind_table[2 * (i - 1)] < player.id) {
            database->ind_table[i * 2] = player.id;
            database->ind_table[i * 2 + 1] = database->cnt_players - 1;
            break;
        }
        else {
            database->ind_table[i * 2] =  database->ind_table[2 * (i - 1)];
            database->ind_table[i * 2 + 1] =  database->ind_table[2 * (i - 1) + 1];
        }
    }
    update_index_file(database);
}

void insert_player_to_file(chess_database* database, chess_player player) {
    FILE* player_file_pointer = fopen(database->player_file_path, "r+b");
    unsigned size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * (database->cnt_players - 1);
    fseek(player_file_pointer, offset, SEEK_SET);
    fwrite(&player, sizeof(chess_player), 1, player_file_pointer);
    int additional_info[2] = {-1, 0}; // position of first s, and cnt of sons
    fwrite(additional_info, sizeof(int), 2, player_file_pointer);
    fclose(player_file_pointer);
}

void insert_player(chess_database* database, chess_player player) {
    insert_player_to_index_table(database, player);
    insert_player_to_file(database, player);
}

int get_cnt_members(chess_database * database) {
    FILE* member_file_pointer = fopen(database->member_file_path, "rb");
    int result;
    fread(&result, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
    return result;
}

void insert_member_to_file(chess_database* database, chess_club_member member, int cur_cnt_members, int additional_info) {
    printf("%d\n", cur_cnt_members);
    FILE* member_file_pointer = fopen(database->member_file_path, "r+b");
    cur_cnt_members++;
    fwrite(&cur_cnt_members, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
    member_file_pointer = fopen(database->member_file_path, "r+b");
    unsigned size_of_one_item = sizeof(chess_club_member) + sizeof(int);
    unsigned offset = sizeof(int) + size_of_one_item * (cur_cnt_members - 1);
    fseek(member_file_pointer, offset, SEEK_SET);
    fwrite(&member, sizeof(chess_club_member), 1, member_file_pointer);
    fwrite(&additional_info, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
}

int get_player_row(chess_database* database, int player_id) {
    int left = 0;
    int right = database->cnt_players - 1;
    while (right - left > 1) {
        int middle = (right + left) / 2;
        if (database->ind_table[middle * 2] < player_id)
            left = middle;
        else
            right = middle;
    }
    if (database->ind_table[left * 2] == player_id)
        return database->ind_table[left * 2 + 1];
    if (database->ind_table[right * 2] == player_id)
        return database->ind_table[right * 2 + 1];
//    for (int i = 0; i < database->cnt_players; ++i) {
//        if (database->ind_table[i * 2] == player_id) {
//            return database->ind_table[i * 2 + 1];
//        }
//    }
    return -1;
}

void get_additional_info_by_player_row(chess_database* database, int player_row,
        int* first_member_position, int* cnt_memberships) {
    FILE* player_file_pointer = fopen(database->player_file_path, "rb");
    unsigned size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * (player_row) + sizeof(chess_player);
    fseek(player_file_pointer, offset, SEEK_SET);
    fread(first_member_position, sizeof(int), 1, player_file_pointer);
    fread(cnt_memberships, sizeof(int), 1, player_file_pointer);
    fclose(player_file_pointer);
}

void update_additional_player_info(chess_database* database, int player_row,
                                   int first_member_position, int cnt_memberships) {
    FILE* player_file_pointer = fopen(database->player_file_path, "r+b");
    int size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * (player_row) + sizeof(chess_player);
    fseek(player_file_pointer, offset, SEEK_SET);
    fwrite(&first_member_position, sizeof(int), 1, player_file_pointer);
    fwrite(&cnt_memberships, sizeof(int), 1, player_file_pointer);
    fclose(player_file_pointer);
}

int update_player_after_insertion(chess_database* database, int player_row, int member_row) {
    int position, cnt_memberships;
    get_additional_info_by_player_row(database, player_row, &position, &cnt_memberships);
    ++cnt_memberships;
    update_additional_player_info(database, player_row, member_row, cnt_memberships);
    return position;
}

void insert_member(chess_database* database, chess_club_member member) {
    int cnt_members = get_cnt_members(database);
    int row = get_player_row(database, member.player_id);
    int prev_position = update_player_after_insertion(database, row, cnt_members);
    insert_member_to_file(database, member, cnt_members, prev_position);
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

chess_club_member get_member_from_file(chess_database* database, int start_row, int member_id) {
    FILE* member_file_pointer = fopen(database->member_file_path, "rb");
    int cur_row = start_row;
    int size_of_one_row = sizeof(chess_club_member) + sizeof(int);
    while (cur_row != -1) {
        unsigned offset = size_of_one_row * cur_row + sizeof(int);
        fseek(member_file_pointer, offset, SEEK_SET);
        chess_club_member cur_member;
        fread(&cur_member, sizeof(cur_member), 1, member_file_pointer);
        if (cur_member.id == member_id)
            return cur_member;
        fread(&cur_row, sizeof(int), 1, member_file_pointer);
    }
    chess_club_member member = {-1, -1, "", ""};
    return member;
}

chess_club_member get_member(chess_database* database, int player_id, int member_id) {
    int row = get_player_row(database, player_id);
    if (row == -1) {
        chess_club_member member = {-1, -1, "", ""};
        return member;
    }
    int position, cnt_memberships;
    get_additional_info_by_player_row(database, row, &position, &cnt_memberships);
    return get_member_from_file(database, position, member_id);
}

int get_member_id_by_row(chess_database* database, int row){
    FILE* member_file_pointer = fopen(database->member_file_path, "rb");
    int size_of_one_item = sizeof(int) + sizeof(chess_club_member);
    unsigned offset = size_of_one_item * row + sizeof(int);
    chess_club_member member;
    fseek(member_file_pointer, offset, SEEK_SET);
    fread(&member, sizeof(chess_club_member), 1, member_file_pointer);
    return member.id;
}

void delete_player_memberships(chess_database* database, int player_id) {
    int player_row = get_player_row(database, player_id);
    int cnt_memberships, member_row;
    get_additional_info_by_player_row(database, player_row, &member_row, &cnt_memberships);
    if (member_row == - 1) {
        return;
    }
    int member_id = get_member_id_by_row(database, member_row);
    printf("%d %d\n",member_id, member_row );
    while(member_row != - 1) {
        del_member(database, player_id, member_id);
        get_additional_info_by_player_row(database, player_row, &member_row, &cnt_memberships);
        if (member_row == - 1) {
            return;
        }
        member_id = get_member_id_by_row(database, member_row);
        printf("%d %d\n",member_id, member_row );
    }
}

void update_row_in_index_file(chess_database* database, int player_id, int new_row) {
    for (int i = 0; i < database->cnt_players; ++i) {
        if (database->ind_table[i * 2] == player_id) {
            database->ind_table[i * 2 + 1] = new_row;
        }
    }
}

void shift_players_in_index_file(chess_database* database, int deleted_player_id){
    printf("%d!!!\n", deleted_player_id);
    int start_row = database->cnt_players;
    for (int i = 0; i < database->cnt_players;++i) {
        if (database->ind_table[i * 2] == deleted_player_id) {
            start_row = i;
            printf("%d!!!\n", deleted_player_id);
            break;
        }
    }
    for (int i = start_row; i < database->cnt_players - 1; ++i){
        database->ind_table[i * 2] = database->ind_table[i * 2 + 2];
        database->ind_table[i * 2 + 1] = database->ind_table[i * 2 + 3];
    }
    database->cnt_players--;
}

void move_last_player_to_deleted_row(chess_database* database, int deleted_player_id){
    int player_row = get_player_row(database, deleted_player_id);
    if (player_row != database->cnt_players - 1) {
        chess_player last_player = get_player_by_row(database, database->cnt_players - 1);
        print_chess_player_info(last_player);
        int cnt_memberships, position_first_member;
        get_additional_info_by_player_row(database, database->cnt_players - 1,
                                          &position_first_member, &cnt_memberships);
        FILE *player_file_pointer = fopen(database->player_file_path, "r+b");
        int one_item_size = sizeof(chess_player) + 2 * sizeof(int);
        unsigned offset = one_item_size * player_row;
        fseek(player_file_pointer, offset, SEEK_SET);
        fwrite(&last_player, sizeof(chess_player), 1, player_file_pointer);
        fwrite(&position_first_member, sizeof(int), 1, player_file_pointer);
        fwrite(&cnt_memberships, sizeof(int), 1, player_file_pointer);
        fclose(player_file_pointer);
        update_row_in_index_file(database, last_player.id, player_row);
    }
    printf("111\n");
    shift_players_in_index_file(database, deleted_player_id);
    printf("111\n");
    update_index_file(database);
}

void del_player(chess_database* database, int player_id) {
    delete_player_memberships(database, player_id);
    printf("DEBUG: player memberships deleted");
    move_last_player_to_deleted_row(database, player_id);
}

void update_player_after_member_deleted(chess_database* database, int player_id, int member_row) {
    int position, cnt;
    int row = get_player_row(database, player_id);
    get_additional_info_by_player_row(database, row, &position, &cnt);
    if (member_row == get_cnt_members(database))
        update_additional_player_info(database, row, member_row, cnt);
}

void move_last_item_after_member_deleted(chess_database* database, int deleted_row) {
    int cnt_members = get_cnt_members(database);
    if (cnt_members == deleted_row)
        return;
    FILE* member_file_pointer = fopen(database->member_file_path, "r+b");
    int size_of_one_item = sizeof(chess_club_member) + sizeof(int);
    unsigned offset = size_of_one_item * (cnt_members - 1) + sizeof(int);
    fseek(member_file_pointer, offset, SEEK_SET);
    chess_club_member member;
    int next_member;
    fread(&member, sizeof(chess_club_member), 1, member_file_pointer);
    fread(&next_member, sizeof(int), 1, member_file_pointer);
    offset = size_of_one_item * (deleted_row) + sizeof(int);
    fseek(member_file_pointer, offset, SEEK_SET);
    fwrite(&member, sizeof(chess_club_member), 1, member_file_pointer);
    fwrite(&next_member, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
    update_player_after_member_deleted(database, member.player_id, deleted_row);
}

void dec_cnt_sons(chess_database* database, int player_id) {
    int position, cnt;
    int row = get_player_row(database, player_id);
    get_additional_info_by_player_row(database, row, &position, &cnt);
    if (cnt == 1)
        position = -1;
    update_additional_player_info(database, row, position, cnt - 1);
}

void delete_member_from_file(chess_database* database, int player_id, int member_id, int start_row) {
    FILE* member_file_pointer = fopen(database->member_file_path, "r+b");
    int cur_row = start_row;
    int prev_row = -1;
    int size_of_one_row = sizeof(chess_club_member) + sizeof(int);
    int k = 0;
    while (cur_row != -1 && k++ < 10) {
        unsigned offset = size_of_one_row * cur_row + sizeof(int);
        fseek(member_file_pointer, offset, SEEK_SET);
        chess_club_member cur_member;
        fread(&cur_member, sizeof(chess_club_member), 1, member_file_pointer);
        if (cur_member.id == member_id){
            int next_row;
            fread(&next_row, sizeof(int), 1, member_file_pointer);
            if (prev_row == -1) {
                //update player field;
                update_player_after_member_deleted(database, player_id, next_row);
            }
            else {
                // update prev member ptr
                offset = size_of_one_row * prev_row + sizeof(int) + sizeof(chess_club_member);
                fseek(member_file_pointer, offset, SEEK_SET);
                fwrite(&next_row, sizeof(int), 1, member_file_pointer);
            }
            fclose(member_file_pointer);
            move_last_item_after_member_deleted(database, cur_row);
            return;
        }
        prev_row = cur_row;
        fread(&cur_row, sizeof(int), 1, member_file_pointer);
    }
    fclose(member_file_pointer);
}

void set_cnt_members(chess_database* database, int value) {
    FILE* member_file_pointer = fopen(database->member_file_path, "r+b");
    fwrite(&value, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
}

void del_member(chess_database* database, int player_id, int member_id) {
    printf("%d %d - - - - \n", player_id, member_id);
    if (get_member(database, player_id, member_id).id == -1)
        return;
    int start_position, cnt_sons;
    int row = get_player_row(database, player_id);
    get_additional_info_by_player_row(database, row, &start_position, &cnt_sons);
    delete_member_from_file(database,player_id,member_id, start_position);
    int cnt_members = get_cnt_members(database);
    dec_cnt_sons(database, player_id);
    set_cnt_members(database, cnt_members - 1);
}

int update_player(chess_database* database, chess_player player) {
    int row = get_player_row(database, player.id);
    if (row == -1)
        return -1;
    FILE* player_file_pointer = fopen(database->player_file_path, "r+b");
    unsigned size_of_one_item = sizeof(chess_player) + 2 * sizeof(int);
    unsigned offset = size_of_one_item * row;
    fseek(player_file_pointer, offset, SEEK_SET);
    fwrite(&player, sizeof(chess_player), 1, player_file_pointer);
    fclose(player_file_pointer);
    return 0;
}

int get_cnt_players(chess_database* database) {
    return database->cnt_players;
}

int get_cnt_player_members(chess_database* database, int player_id) {
    int position, cnt_memberships;
    get_additional_info_by_player_row(database, get_player_row(database, player_id), &position, &cnt_memberships);
    return cnt_memberships;
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
    for (int i = 0; i < cnt_players;++i)
        printf("%d %d\n", table[2 * i], table[2 * i + 1]);
    printf("\n");
    fclose(player_index_file_pointer);
}

void utl_members(chess_database* database) {
    FILE* member_file_pointer = fopen(database->member_file_path, "rb");

    int cnt_members;
    fread(&cnt_members, sizeof(int), 1, member_file_pointer);
    printf("%d\n", cnt_members);
    for (int i = 0; i < cnt_members; ++i) {
        chess_club_member club_member;
        int previous_row;
        fread(&club_member, sizeof(chess_club_member), 1, member_file_pointer);
        fread(&previous_row, sizeof(int), 1, member_file_pointer);
        printf("%d %d %s %s %d\n", club_member.id, club_member.player_id,
                club_member.club_name, club_member.position, previous_row);
    }
    fclose(member_file_pointer);
}

void read_index_file(chess_database* database) {
    printf("reading...\n");
    FILE* player_index_file_pointer = fopen(database->player_index_path, "rb");
    fread(&database->cnt_players, sizeof(int), 1, player_index_file_pointer);
    if (database->cnt_players != 0) {
        fread(&database->ind_table, sizeof(int), database->cnt_players * 2, player_index_file_pointer);
    }
    fclose(player_index_file_pointer);
}

void create_player_index_file(chess_database* database) {
    printf("creating...\n");
    FILE* player_index_file_pointer = fopen(database->player_index_path, "wb");
    int cnt_players = 0;
    fwrite(&cnt_players, sizeof(int), 1, player_index_file_pointer);
    fclose(player_index_file_pointer);
}

void create_player_file(chess_database* database) {
    FILE* player_index_file_pointer = fopen(database->player_file_path, "wb");
    fclose(player_index_file_pointer);
}

void create_member_file(chess_database* database) {
    printf("creating member file...\n");
    printf("%s\n", database->member_file_path);
    FILE* member_file_pointer = fopen(database->member_file_path, "wb");
    int cnt_players = 0;
    fwrite(&cnt_players, sizeof(int), 1, member_file_pointer);
    fclose(member_file_pointer);
}


void init_database(chess_database* database) {
    if( access(database->player_file_path, F_OK ) == -1 )
         create_player_file(database);

    if( access(database->player_index_path, F_OK ) == -1 )
        create_player_index_file(database);

    if( access(database->member_file_path, F_OK ) == -1 )
        create_member_file(database);

    read_index_file(database);
}

int update_member_in_file(chess_database* database, int start_row, chess_club_member member){
    FILE* member_file_pointer = fopen(database->member_file_path, "r+b");
    int cur_row = start_row;
    int size_of_one_row = sizeof(chess_club_member) + sizeof(int);
    while (cur_row != -1) {
        unsigned offset = size_of_one_row * cur_row + sizeof(int);
        fseek(member_file_pointer, offset, SEEK_SET);
        chess_club_member cur_member;
        fread(&cur_member, sizeof(chess_club_member), 1, member_file_pointer);
        if (cur_member.id == member.id){
            fseek(member_file_pointer, offset, SEEK_SET);
            fwrite(&member, sizeof(chess_club_member), 1, member_file_pointer);
            fclose(member_file_pointer);
            return 0;
        }
        fread(&cur_row, sizeof(int), 1, member_file_pointer);
    }
    fclose(member_file_pointer);
    return -1;
}

int update_member(chess_database* database, chess_club_member member) {
    int row = get_player_row(database, member.player_id);
    if (row == -1)
        return -1;
    int position, cnt_memberships;
    get_additional_info_by_player_row(database, row, &position, &cnt_memberships);
    return update_member_in_file(database, position, member);

}

//TODO

