
#ifndef LIST_H
#define LIST_H

#include "room.h"

void init_room_list(RoomList *list);

int add_room(RoomList *list, Room room);

Room *find_room(RoomList *list, int room_number);

int remove_room(RoomList *list, int room_number);

void free_room_list(RoomList *list);

#endif
