#include <stdio.h>
#include <string.h>
#include "search.h"

Node *search_room_by_number(RoomList *list, int room_number) {
    Node *curr = list->head;
    while (curr) {
        if (curr->data.room_number == room_number) return curr;
        curr = curr->next;
    }
    return NULL;
}

int search_guest_by_name(RoomList *list, const char *guest_name) {
    Node *curr = list->head;
    int count = 0;
    while (curr) {
        if (strcmp(curr->data.guest_name, guest_name) == 0) {
            printf("Room %d: %s\n", curr->data.room_number, curr->data.guest_name);
            count++;
        }
        curr = curr->next;
    }
    return count;
}

int search_rooms_by_type(RoomList *list, RoomType type) {
    Node *curr = list->head;
    int count = 0;
    while (curr) {
        if (curr->data.type == type) count++;
        curr = curr->next;
    }
    return count;
}

int search_rooms_by_status(RoomList *list, RoomStatus status) {
    Node *curr = list->head;
    int count = 0;
    while (curr) {
        if (curr->data.status == status) count++;
        curr = curr->next;
    }
    return count;
}

double calculate_total_revenue(RoomList *list) {
    Node *curr = list->head;
    double total = 0.0;
    while (curr) {
        if (curr->data.status == ROOM_OCCUPIED) {
            total += curr->data.price_per_night * curr->data.nights_booked;
        }
        curr = curr->next;
    }
    return total;
}

void generate_daily_report(RoomList *list) {
    int occ = search_rooms_by_status(list, ROOM_OCCUPIED);
    double rev = calculate_total_revenue(list);
    printf("\nOccupied Rooms: %d / %d | Revenue: %.2f\n", occ, list->total_rooms, rev);
}
