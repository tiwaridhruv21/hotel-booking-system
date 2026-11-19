#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "search.h"

static void make_lowercase(const char *src, char *dst, int size) {
    if (src == NULL || dst == NULL || size <= 0) {
        return;
    }

    int i = 0;
    while (src[i] != '\0' && i < size - 1) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

Node *search_room_by_number(RoomList *list, int room_number) {
    if (list == NULL || room_number <= 0) {
        return NULL;
    }

    Node *curr = list->head;
    while (curr != NULL) {
        if (curr->data.room_number == room_number) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

int search_guest_by_name(RoomList *list, const char *guest_name) {
    if (list == NULL || guest_name == NULL || guest_name[0] == '\0') {
        return 0;
    }

    Node *curr = list->head;
    char query[50];
    char name_buf[50];
    int count = 0;

    make_lowercase(guest_name, query, sizeof(query));

    while (curr != NULL) {
        if (curr->data.status == ROOM_OCCUPIED) {
            make_lowercase(curr->data.guest_name, name_buf, sizeof(name_buf));
            if (strstr(name_buf, query) != NULL) {
                printf("Room %d | Guest: %s | Nights: %d | Rate: %.2f\n",
                       curr->data.room_number,
                       curr->data.guest_name,
                       curr->data.nights_booked,
                       curr->data.price_per_night);
                count++;
            }
        }
        curr = curr->next;
    }
    return count;
}

int search_rooms_by_type(RoomList *list, RoomType type) {
    if (list == NULL) {
        return 0;
    }

    Node *curr = list->head;
    int count = 0;

    while (curr != NULL) {
        if (curr->data.type == type) {
            const char *st = (curr->data.status == ROOM_AVAILABLE) ? "Available" :
                             (curr->data.status == ROOM_OCCUPIED) ? "Occupied" : "Maintenance";
            printf("Room %d | Rate: %.2f | Status: %s | Guest: %s\n",
                   curr->data.room_number,
                   curr->data.price_per_night,
                   st,
                   curr->data.guest_name);
            count++;
        }
        curr = curr->next;
    }
    return count;
}

int search_rooms_by_status(RoomList *list, RoomStatus status) {
    if (list == NULL) {
        return 0;
    }

    Node *curr = list->head;
    int count = 0;

    while (curr != NULL) {
        if (curr->data.status == status) {
            printf("Room %d | Rate: %.2f | Guest: %s\n",
                   curr->data.room_number,
                   curr->data.price_per_night,
                   curr->data.guest_name);
            count++;
        }
        curr = curr->next;
    }
    return count;
}

double calculate_total_revenue(RoomList *list) {
    if (list == NULL) {
        return 0.0;
    }

    Node *curr = list->head;
    double total = 0.0;

    while (curr != NULL) {
        if (curr->data.status == ROOM_OCCUPIED) {
            total += curr->data.price_per_night * curr->data.nights_booked;
        }
        curr = curr->next;
    }
    return total;
}

void generate_daily_report(RoomList *list) {
    if (list == NULL) {
        printf("\nUnable to generate report.\n");
        return;
    }

    Node *curr = list->head;
    int total = 0;
    int available = 0;
    int occupied = 0;
    int maintenance = 0;
    double revenue = calculate_total_revenue(list);

    while (curr != NULL) {
        total++;
        if (curr->data.status == ROOM_AVAILABLE) {
            available++;
        } else if (curr->data.status == ROOM_OCCUPIED) {
            occupied++;
        } else {
            maintenance++;
        }
        curr = curr->next;
    }

    double rate = (total > 0) ? ((double)occupied / total) * 100.0 : 0.0;

    printf("\n--- End-of-Day Occupancy & Revenue Report ---\n");
    printf("Total Rooms:       %d\n", total);
    printf("Available Rooms:   %d\n", available);
    printf("Occupied Rooms:    %d\n", occupied);
    printf("Maintenance Rooms: %d\n", maintenance);
    printf("Occupancy Rate:    %.1f%%\n", rate);
    printf("Active Revenue:    %.2f\n", revenue);
}
