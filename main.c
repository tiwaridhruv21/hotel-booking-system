
#include <stdio.h>

#include "room.h"
#include "list.h"
#include "file_io.h"
#include "ui.h"

/* Main function */
int main(void)
{
    RoomList list;
    int choice;

    /* Initialize the room list */
    init_room_list(&list);

    /* Load existing rooms from file */
    load_rooms_from_file(&list);

    printf("\nHotel Booking System Started!\n");

    do
    {
        choice = main_menu();

        switch (choice)
        {
            case 1:
                handle_room_list_menu(&list);
                break;

            case 2:
   		 handle_add_room(&list);
  		 break;

            case 3:
                handle_search_menu(&list);
                break;

            case 4:
                handle_report_menu(&list);
                break;
            case 5:
                printf("\nSaving hotel records...\n");
                save_rooms_to_file(&list);
                printf("Exiting Hotel Booking System.\n");
                break;

            case -1:
                printf("\nInput ended. Saving hotel records...\n");
                save_rooms_to_file(&list);
                break;
        }

    } while (choice != 5 && choice != -1);

    /* Free allocated memory */
    free_room_list(&list);

    return 0;
}

