#ifndef BOOKING_H
#define BOOKING_H

#include "room.h"

int check_in_guest(Room *r, const char *name, int nights);
double check_out_guest(Room *r);
int change_room_status(Room *r, RoomStatus status);
int cancel_reservation(Room *r);

#endif
