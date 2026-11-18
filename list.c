
#include <stdlib.h>
#include "list.h"

// Initialize an empty linked list
void init_room_list(RoomList *list) {
    if (list == NULL) return;

    list->head = NULL;
    list->total_rooms = 0;
}

// Find a room by its room number
Room *find_room(RoomList *list, int room_number) {
    if (list == NULL) return NULL;

    Node *current = list->head;

    while (current != NULL) {
        if (current->data.room_number == room_number) {
            return &current->data;
        }
        current = current->next;
    }

    return NULL;
}

// Add a new room to the linked list
int add_room(RoomList *list, Room room) {
    if (list == NULL) return 0;

    // Prevent duplicate room numbers
    if (find_room(list, room.room_number) != NULL) {
        return 0;
    }

    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        return 0;
    }

    new_node->data = room;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = new_node;
    }

    list->total_rooms++;
    return 1;
}

// Remove a room from the linked list
int remove_room(RoomList *list, int room_number) {
    if (list == NULL) return 0;

    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (current->data.room_number == room_number) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            list->total_rooms--;
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

// Free all nodes before exiting
void free_room_list(RoomList *list) {
    if (list == NULL) return;

    Node *current = list->head;

    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->total_rooms = 0;
}
