#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "list.h"
#include "booking.h"
#include "search.h"

void get_string_input(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}

int get_int_input(const char *prompt) {
    char buf[64];
    char *end;
    while (1) {
        get_string_input(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') {
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        long val = strtol(buf, &end, 10);
        if (*end == '\0') {
            return (int)val;
        }
        printf("Invalid input. Enter a valid integer.\n");
    }
}

double get_double_input(const char *prompt) {
    char buf[64];
    char *end;
    while (1) {
        get_string_input(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') {
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        double val = strtod(buf, &end);
        if (*end == '\0') {
            return val;
        }
        printf("Invalid input. Enter a valid amount.\n");
    }
}

static const char *type_to_str(RoomType type) {
    if (type == ROOM_SINGLE) return "Single";
    if (type == ROOM_DOUBLE) return "Double";
    return "Suite";
}

static const char *status_to_str(RoomStatus status) {
    if (status == ROOM_AVAILABLE) return "Available";
    if (status == ROOM_OCCUPIED) return "Occupied";
    return "Maintenance";
}

void display_all_rooms(const RoomList *list, int filter) {
    Node *curr = list->head;
    int count = 0;

    printf("\n%-8s %-10s %-12s %-8s %-14s %-20s\n",
           "Room#", "Type", "Rate", "Nights", "Status", "Guest");
    printf("--------------------------------------------------------------------------\n");

    while (curr != NULL) {
        Room r = curr->data;
        if (filter == 0 ||
            (filter == 1 && r.status == ROOM_AVAILABLE) ||
            (filter == 2 && r.status == ROOM_OCCUPIED) ||
            (filter == 3 && r.status == ROOM_MAINTENANCE)) {
            printf("%-8d %-10s %-12.2f %-8d %-14s %-20s\n",
                   r.room_number,
                   type_to_str(r.type),
                   r.price_per_night,
                   r.nights_booked,
                   status_to_str(r.status),
                   r.guest_name);
            count++;
        }
        curr = curr->next;
    }

    if (count == 0) {
        printf("No rooms found matching selection.\n");
    }
}

void room_operations_menu(Room *room) {
    int choice = 0;
    while (choice != 6) {
        printf("\n--- Room %d Operations ---\n", room->room_number);
        printf("1. View Room Details & Guest Information\n");
        printf("2. Check-In Guest\n");
        printf("3. Check-Out Guest\n");
        printf("4. Change Room Status (Maintenance / Available)\n");
        printf("5. Cancel Reservation\n");
        printf("6. Go back to Main Menu\n");
        choice = get_int_input("Enter choice: ");

        if (choice == 1) {
            printf("\nRoom Number: %d\n", room->room_number);
            printf("Room Type:   %s\n", type_to_str(room->type));
            printf("Rate/Night:  %.2f\n", room->price_per_night);
            printf("Status:      %s\n", status_to_str(room->status));
            printf("Guest Name:  %s\n", room->guest_name);
            printf("Nights:      %d\n", room->nights_booked);
        } else if (choice == 2) {
            char name[50];
            get_string_input("Enter guest name: ", name, sizeof(name));
            int nights = get_int_input("Enter number of nights: ");
            if (check_in_guest(room, name, nights)) {
                printf("Guest %s checked into room %d.\n", room->guest_name, room->room_number);
            } else {
                printf("Check-in failed. Room may not be available or input is invalid.\n");
            }
        } else if (choice == 3) {
            double bill = check_out_guest(room);
            if (bill >= 0.0) {
                printf("Checked out successfully. Total bill: %.2f\n", bill);
            } else {
                printf("Check-out failed. Room is not currently occupied.\n");
            }
        } else if (choice == 4) {
            int st = get_int_input("Set status (1 = Available, 2 = Maintenance): ");
            RoomStatus target = (st == 2) ? ROOM_MAINTENANCE : ROOM_AVAILABLE;
            if (st != 1 && st != 2) {
                printf("Invalid status option.\n");
            } else if (change_room_status(room, target)) {
                printf("Room %d status updated to %s.\n", room->room_number, status_to_str(room->status));
            } else {
                printf("Cannot change status while room is occupied.\n");
            }
        } else if (choice == 5) {
            if (cancel_reservation(room)) {
                printf("Reservation cancelled. Room %d is now available.\n", room->room_number);
            } else {
                printf("No active reservation to cancel.\n");
            }
        } else if (choice != 6) {
            printf("Invalid option.\n");
        }
    }
}

void main_menu(RoomList *list) {
    int choice = 0;
    while (choice != 5) {
        printf("\n=== Hotel Reception Desk ===\n");
        printf("1. List All Rooms\n");
        printf("2. Add a New Room to Inventory\n");
        printf("3. Select / Search Room or Guest Name\n");
        printf("4. Generate End-of-Day Occupancy & Revenue Report\n");
        printf("5. Save & Exit\n");
        choice = get_int_input("Enter choice: ");

        if (choice == 1) {
            int filter = get_int_input("Filter (0 = All, 1 = Available, 2 = Occupied, 3 = Maintenance): ");
            if (filter < 0 || filter > 3) {
                filter = 0;
            }
            display_all_rooms(list, filter);
        } else if (choice == 2) {
            Room r;
            r.room_number = get_int_input("Enter new room number: ");
            if (find_room(list, r.room_number) != NULL) {
                printf("Room %d already exists.\n", r.room_number);
                continue;
            }
            int t = get_int_input("Room type (0 = Single, 1 = Double, 2 = Suite): ");
            if (t == 1) r.type = ROOM_DOUBLE;
            else if (t == 2) r.type = ROOM_SUITE;
            else r.type = ROOM_SINGLE;

            r.price_per_night = get_double_input("Enter price per night: ");
            if (r.price_per_night < 0.0) {
                printf("Price cannot be negative.\n");
                continue;
            }
            strcpy(r.guest_name, "N/A");
            r.nights_booked = 0;
            r.status = ROOM_AVAILABLE;

            if (add_room(list, r)) {
                printf("Room %d added to inventory.\n", r.room_number);
            } else {
                printf("Failed to add room.\n");
            }
        } else if (choice == 3) {
            int sub = get_int_input("Search by (1 = Room Number, 2 = Guest Name, 3 = Room Type): ");
            if (sub == 1) {
                int num = get_int_input("Enter room number: ");
                Room *found = find_room(list, num);
                if (found != NULL) {
                    room_operations_menu(found);
                } else {
                    printf("Room %d not found.\n", num);
                }
            } else if (sub == 2) {
                char query[50];
                get_string_input("Enter guest name: ", query, sizeof(query));
                if (search_guest_by_name(list, query) == 0) {
                    printf("No matching guest found.\n");
                }
            } else if (sub == 3) {
                int t = get_int_input("Room type (0 = Single, 1 = Double, 2 = Suite): ");
                RoomType rt = (t == 1) ? ROOM_DOUBLE : (t == 2 ? ROOM_SUITE : ROOM_SINGLE);
                if (search_rooms_by_type(list, rt) == 0) {
                    printf("No rooms of that type found.\n");
                }
            } else {
                printf("Invalid search option.\n");
            }
        } else if (choice == 4) {
            generate_daily_report(list);
        } else if (choice != 5) {
            printf("Invalid choice.\n");
        }
    }
}
