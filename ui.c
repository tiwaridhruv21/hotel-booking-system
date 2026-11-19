#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "list.h"
#include "booking.h"
#include "search.h"

static void read_str(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    }
}

static int read_int(const char *prompt) {
    char buf[32];
    read_str(prompt, buf, sizeof(buf));
    return atoi(buf);
}

static double read_double(const char *prompt) {
    char buf[32];
    read_str(prompt, buf, sizeof(buf));
    return atof(buf);
}

static void list_rooms(RoomList *list) {
    Node *curr = list->head;
    printf("\nRoom | Type | Rate | Nights | Status | Guest\n");
    while (curr) {
        Room r = curr->data;
        const char *st = (r.status == ROOM_AVAILABLE) ? "Available" :
                         (r.status == ROOM_OCCUPIED) ? "Occupied" : "Maintenance";
        printf("%d | %d | %.2f | %d | %s | %s\n",
               r.room_number, r.type, r.price_per_night, r.nights_booked, st, r.guest_name);
        curr = curr->next;
    }
}

static void room_menu(Room *r) {
    int ch = 0;
    while (ch != 5) {
        printf("\n--- Room %d ---\n", r->room_number);
        printf("1. Check-In\n2. Check-Out\n3. Toggle Maintenance\n4. Cancel Booking\n5. Back\n");
        ch = read_int("Choice: ");

        if (ch == 1) {
            char name[50];
            read_str("Guest name: ", name, sizeof(name));
            int nights = read_int("Nights: ");
            if (check_in_guest(r, name, nights)) printf("Checked in.\n");
            else printf("Failed (room unavailable or bad input).\n");
        } else if (ch == 2) {
            double bill = check_out_guest(r);
            if (bill >= 0) printf("Checked out. Bill: %.2f\n", bill);
            else printf("Room not occupied.\n");
        } else if (ch == 3) {
            RoomStatus next = (r->status == ROOM_MAINTENANCE) ? ROOM_AVAILABLE : ROOM_MAINTENANCE;
            if (change_room_status(r, next)) printf("Status updated.\n");
            else printf("Cannot change status while occupied.\n");
        } else if (ch == 4) {
            if (cancel_reservation(r)) printf("Cancelled.\n");
            else printf("Nothing to cancel.\n");
        }
    }
}

void main_menu(RoomList *list) {
    int ch = 0;
    while (ch != 5) {
        printf("\n=== Hotel Menu ===\n");
        printf("1. List Rooms\n2. Add Room\n3. Manage Room\n4. Daily Report\n5. Save & Exit\n");
        ch = read_int("Choice: ");

        if (ch == 1) {
            list_rooms(list);
        } else if (ch == 2) {
            Room r;
            r.room_number = read_int("Room number: ");
            if (find_room(list, r.room_number)) {
                printf("Room exists.\n");
                continue;
            }
            r.type = (RoomType)read_int("Type (0=Single, 1=Double, 2=Suite): ");
            r.price_per_night = read_double("Price/night: ");
            strcpy(r.guest_name, "N/A");
            r.nights_booked = 0;
            r.status = ROOM_AVAILABLE;
            add_room(list, r);
            printf("Room added.\n");
        } else if (ch == 3) {
            int num = read_int("Enter room number: ");
            Room *r = find_room(list, num);
            if (r) room_menu(r);
            else printf("Not found.\n");
        } else if (ch == 4) {
            generate_daily_report(list);
        }
    }
}
