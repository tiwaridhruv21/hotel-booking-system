#include "booking.h"
#include <string.h>

int check_in_guest(Room *r, const char *name, int nights) {
    if (!r || !name || nights <= 0 || name[0] == '\0') {
        return 0;
    }
    if (r->status != ROOM_AVAILABLE) {
        return 0;
    }

    strncpy(r->guest_name, name, 49);
    r->guest_name[49] = '\0';
    r->nights_booked = nights;
    r->status = ROOM_OCCUPIED;
    return 1;
}

double check_out_guest(Room *r) {
    if (!r || r->status != ROOM_OCCUPIED) {
        return -1.0;
    }

    double bill = r->price_per_night * r->nights_booked;
    strcpy(r->guest_name, "N/A");
    r->nights_booked = 0;
    r->status = ROOM_AVAILABLE;
    return bill;
}

int change_room_status(Room *r, RoomStatus status) {
    if (!r || r->status == ROOM_OCCUPIED) {
        return 0;
    }
    if (status != ROOM_AVAILABLE && status != ROOM_MAINTENANCE) {
        return 0;
    }

    r->status = status;
    if (status == ROOM_AVAILABLE) {
        strcpy(r->guest_name, "N/A");
        r->nights_booked = 0;
    }
    return 1;
}

int cancel_reservation(Room *r) {
    if (!r || r->status != ROOM_OCCUPIED) {
        return 0;
    }

    strcpy(r->guest_name, "N/A");
    r->nights_booked = 0;
    r->status = ROOM_AVAILABLE;
    return 1;
}
