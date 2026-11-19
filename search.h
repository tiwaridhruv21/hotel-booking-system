#ifndef SEARCH_H
#define SEARCH_H

#include "room.h"

/*
 * Search functions
 */

/* Returns the room node if found, otherwise NULL. */
Node *search_room_by_number(RoomList *list, int room_number);

/* Case-insensitive search for guest name.
 * Displays all matching guests.
 * Returns the number of matches found.
 */
int search_guest_by_name(RoomList *list, const char *guest_name);

/* Displays all rooms matching the given room type.
 * Returns the number of matches found.
 */
int search_rooms_by_type(RoomList *list, RoomType type);

/* Displays all rooms matching the given status.
 * Returns the number of matches found.
 */
int search_rooms_by_status(RoomList *list, RoomStatus status);

/* Calculates revenue from occupied rooms. */
double calculate_total_revenue(RoomList *list);

/* Displays occupancy and revenue information. */
void generate_daily_report(RoomList *list);

#endif
```