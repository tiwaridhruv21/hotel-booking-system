#ifndef UI_H
#define UI_H

#include "room.h"

void get_string_input(const char *prompt, char *buf, int size);
int get_int_input(const char *prompt);
double get_double_input(const char *prompt);
void display_all_rooms(const RoomList *list, int filter);
void room_operations_menu(Room *room);
void main_menu(RoomList *list);

#endif
