#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "list.h"
#include "booking.h"
#include "search.h"

static void read_line(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) != NULL) {
        int len = (int)strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

static int read_int(const char *prompt) {
    char buf[64];
    int val;
    char extra;
    while (1) {
        read_line(prompt, buf, sizeof(buf));
        if (sscanf(buf, "%d %c", &val, &extra) == 1) {
            return val;
        }
        printf("Invalid number, try again.\n");
    }
}

static double read_double(const char *prompt) {
    char buf[64];
    double val;
    char extra;
    while (1) {
        read_line(prompt, buf, sizeof(buf));
        if (sscanf(buf, "%lf %c", &val, &extra) == 1) {
            return val;
        }
        printf("Invalid amount, try again.\n");
    }
}

static const char *get_type_name(RoomType type) {
    if (type == ROOM_SINGLE) return "Single";
    if (type == ROOM_DOUBLE) return "Double";
    return "Suite";
}

static const char *get_status_name(RoomStatus status) {
    if (status == ROOM_AVAILABLE) return "Available";
    if (status == ROOM_OCCUPIED) return "Occupied";
    return "Maintenance";
}

static void list_rooms(RoomList *list, int filter) {
    Node *curr = list->head;
    int found = 0;

    printf("\n%-8s %-10s %-10s %-8s %-14s %-20s\n",
           "Room", "Type", "Rate", "Nights", "Status", "Guest");
    printf("------------------------------------------------------------------------\n");

    while (curr != NULL) {
        Room r = curr->data;
        int match = 0;
        if (filter == 0) match = 1;
        else if (filter == 1 && r.status == ROOM_AVAILABLE) match = 1;
        else if (filter == 2 && r.status == ROOM_OCCUPIED) match = 1;
        else if (filter == 3 && r.status == ROOM_MAINTENANCE) match = 1;

        if (match) {
            printf("%-8d %-10s %-10.2f %-8d %-14s %-20s\n",
                   r.room_number,
                   get_type_name(r.type),
                   r.price_per_night,
                   r.nights_booked,
                   get_status_name(r.status),
                   r.guest_name);
            found++;
        }
        curr = curr->next;
    }

    if (found == 0) {
        printf("No rooms found.\n");
    }
}

static void room_operations_menu(Room *r) {
    int choice = 0;
    while (choice != 6) {
        printf("\n--- Room %d Operations ---\n", r->room_number);
        printf("1. View Room Details & Guest Information\n");
        printf("2. Check-In Guest\n");
        printf("3. Check-Out Guest\n");
        printf("4. Change Room Status (Maintenance / Available)\n");
        printf("5. Cancel Reservation\n");
        printf("6. Go back to Main Menu\n");
        choice = read_int("Enter choice: ");

        if (choice == 1) {
            printf("\nRoom Number: %d\n", r->room_number);
            printf("Room Type:   %s\n", get_type_name(r->type));
            printf("Rate/Night:  %.2f\n", r->price_per_night);
            printf("Status:      %s\n", get_status_name(r->status));
            printf("Guest Name:  %s\n", r->guest_name);
            printf("Nights:      %d\n", r->nights_booked);
        } else if (choice == 2) {
            char name[50];
            read_line("Enter guest name: ", name, sizeof(name));
            int nights = read_int("Enter number of nights: ");
            if (check_in_guest(r, name, nights)) {
                printf("Guest %s checked into Room %d.\n", r->guest_name, r->room_number);
            } else {
                printf("Check-in failed. Room is unavailable or input is invalid.\n");
            }
        } else if (choice == 3) {
            double bill = check_out_guest(r);
            if (bill >= 0.0) {
                printf("Check-out complete. Total bill: %.2f\n", bill);
            } else {
                printf("Check-out failed. Room is not occupied.\n");
            }
        } else if (choice == 4) {
            int opt = read_int("Set status (1 = Available, 2 = Maintenance): ");
            if (opt != 1 && opt != 2) {
                printf("Invalid status option.\n");
            } else {
                RoomStatus st = (opt == 2) ? ROOM_MAINTENANCE : ROOM_AVAILABLE;
                if (change_room_status(r, st)) {
                    printf("Room %d status updated to %s.\n", r->room_number, get_status_name(r->status));
                } else {
                    printf("Cannot change status while room is occupied.\n");
                }
            }
        } else if (choice == 5) {
            if (cancel_reservation(r)) {
                printf("Reservation cancelled. Room %d is now available.\n", r->room_number);
            } else {
                printf("No active reservation to cancel.\n");
            }
        } else if (choice != 6) {
            printf("Invalid choice.\n");
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
        choice = read_int("Enter choice: ");

        if (choice == 1) {
            int filter = read_int("Filter (0 = All, 1 = Available, 2 = Occupied, 3 = Maintenance): ");
            if (filter < 0 || filter > 3) {
                filter = 0;
            }
            list_rooms(list, filter);
        } else if (choice == 2) {
            Room r;
            r.room_number = read_int("Enter room number: ");
            if (find_room(list, r.room_number) != NULL) {
                printf("Room %d already exists.\n", r.room_number);
                continue;
            }
            int t = read_int("Room type (0 = Single, 1 = Double, 2 = Suite): ");
            if (t == 1) r.type = ROOM_DOUBLE;
            else if (t == 2) r.type = ROOM_SUITE;
            else r.type = ROOM_SINGLE;

            r.price_per_night = read_double("Enter price per night: ");
            if (r.price_per_night < 0.0) {
                printf("Price cannot be negative.\n");
                continue;
            }
            strcpy(r.guest_name, "N/A");
            r.nights_booked = 0;
            r.status = ROOM_AVAILABLE;

            if (add_room(list, r)) {
                printf("Room %d added.\n", r.room_number);
            } else {
                printf("Failed to add room.\n");
            }
        } else if (choice == 3) {
            int sub = read_int("1 = Select Room by Number, 2 = Search Guest Name, 3 = Search Room Type: ");
            if (sub == 1) {
                int num = read_int("Enter room number: ");
                Room *r = find_room(list, num);
                if (r != NULL) {
                    room_operations_menu(r);
                } else {
                    printf("Room %d not found.\n", num);
                }
            } else if (sub == 2) {
                char name[50];
                read_line("Enter guest name: ", name, sizeof(name));
                if (search_guest_by_name(list, name) == 0) {
                    printf("No matching guest found.\n");
                }
            } else if (sub == 3) {
                int t = read_int("Room type (0 = Single, 1 = Double, 2 = Suite): ");
                RoomType rt = (t == 1) ? ROOM_DOUBLE : (t == 2 ? ROOM_SUITE : ROOM_SINGLE);
                if (search_rooms_by_type(list, rt) == 0) {
                    printf("No matching rooms found.\n");
                }
            } else {
                printf("Invalid option.\n");
            }
        } else if (choice == 4) {
            generate_daily_report(list);
        } else if (choice != 5) {
            printf("Invalid choice.\n");
        }
    }
}
