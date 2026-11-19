#ifndef UI_H
#define UI_H

#include "room.h"

/* Menu functions */
int main_menu(void);
int room_operations_menu(void);
int room_list_menu(void);
int search_menu(void);
int report_menu(void);

/* Input handling */
int get_integer(const char *message, int min, int max);
int get_string(const char *message, char *buffer, int size);

/* Room display */
void display_all_rooms(RoomList *list);
void display_rooms_by_status(RoomList *list, RoomStatus status);

/* Menu handlers */
void handle_search_menu(RoomList *list);
void handle_room_list_menu(RoomList *list);
void handle_report_menu(RoomList *list);
void handle_room_operations(RoomList *list);
void handle_add_room(RoomList *list);

#endif
