#include <stdio.h>
#include <string.h>
#include "room.h"
#include "list.h"
#include "search.h"

int main(void) {
    RoomList list;
    init_room_list(&list);

    int choice;
    while (1) {
        printf("\n=== Main Reception Desk Menu ===\n");
        printf("1. List All Rooms\n");
        printf("2. Add a New Room to Inventory\n");
        printf("3. Select / Search Room or Guest Name\n");
        printf("4. Generate End-of-Day Occupancy & Revenue Report\n");
        printf("5. Save & Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                // TODO (Task 4 - UI): Display all rooms
                break;

            case 2:
                // TODO (Task 1 & 4): Add a new room
                break;

            /* ================= YOUR PART (TASK 5: SEARCH) ================= */
            case 3: {
                int search_choice;
                printf("\n--- Search Menu ---\n");
                printf("1. Search by Room Number (Select Room)\n");
                printf("2. Search by Guest Name\n");
                printf("3. Filter by Room Type\n");
                printf("4. Filter by Room Status\n");
                printf("Enter choice: ");

                if (scanf("%d", &search_choice) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input.\n");
                    break;
                }
                while (getchar() != '\n');

                if (search_choice == 1) {
                    int r_num;
                    printf("Enter Room Number: ");
                    if (scanf("%d", &r_num) == 1) {
                        while (getchar() != '\n');
                        Node *selected = search_room_by_number(&list, r_num);
                        if (selected != NULL) {
                            printf("Room %d found! Price: %.2f\n",
                                   selected->data.room_number,
                                   selected->data.price_per_night);
                            // TODO (Task 4 - UI): Open Room Operations Menu with 'selected'
                        } else {
                            printf("Room %d not found.\n", r_num);
                        }
                    }
                } else if (search_choice == 2) {
                    char name[50];
                    printf("Enter Guest Name: ");
                    if (fgets(name, sizeof(name), stdin) != NULL) {
                        name[strcspn(name, "\n")] = '\0';
                        search_guest_by_name(&list, name);
                    }
                } else if (search_choice == 3) {
                    int type;
                    printf("Enter Type (0: Single, 1: Double, 2: Suite): ");
                    if (scanf("%d", &type) == 1 && type >= 0 && type <= 2) {
                        while (getchar() != '\n');
                        search_rooms_by_type(&list, (RoomType)type);
                    } else {
                        while (getchar() != '\n');
                        printf("Invalid room type.\n");
                    }
                } else if (search_choice == 4) {
                    int status;
                    printf("Enter Status (0: Available, 1: Occupied, 2: Maintenance): ");
                    if (scanf("%d", &status) == 1 && status >= 0 && status <= 2) {
                        while (getchar() != '\n');
                        search_rooms_by_status(&list, (RoomStatus)status);
                    } else {
                        while (getchar() != '\n');
                        printf("Invalid status.\n");
                    }
                } else {
                    printf("Invalid search choice.\n");
                }
                break;
            }

            /* ================= YOUR PART (TASK 5: REPORT) ================= */
            case 4:
                generate_daily_report(&list);
                break;

            case 5:
                // TODO (Task 2 - File I/O): Save rooms to disk
                free_room_list(&list);
                printf("Exiting system.\n");
                return 0;

            default:
                printf("Invalid menu option.\n");
        }
    }

    return 0;
}
