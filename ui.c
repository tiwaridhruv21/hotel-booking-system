
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "booking.h"
#include "list.h"
#include "ui.h"
#include "search.h"

/* Safe string input */
int get_string(const char *message, char *buffer, int size)
{
    size_t len;
    int ch;

    if (buffer == NULL || size <= 1)
        return 0;

    printf("%s", message);

    if (fgets(buffer, size, stdin) == NULL)
        return 0;

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    else if (!feof(stdin))
    {
        ch = getchar();

        if (ch != '\n' && ch != EOF)
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                /* Clear remaining input */
            }

            printf("Input too long. Try again.\n");
            return 0;
        }
    }

    return 1;
}


/* Safe integer input */
int get_integer(const char *message, int min, int max)
{
    char buffer[100];
    char *end;
    long number;

    while (1)
    {
        if (!get_string(message, buffer, sizeof(buffer)))
        {
            if (feof(stdin))
                return -1;

            continue;
        }

        errno = 0;
        number = strtol(buffer, &end, 10);

        while (*end == ' ' || *end == '\t')
            end++;

        if (end == buffer || *end != '\0' || errno == ERANGE)
        {
            printf("Invalid input. Enter a number.\n");
            continue;
        }

        if (number < min || number > max)
        {
            printf("Enter a number between %d and %d.\n",
                   min, max);
            continue;
        }

        return (int)number;
    }
}


/* Main reception menu */
int main_menu(void)
{
    printf("\n========== HOTEL BOOKING SYSTEM ==========\n");
    printf("1. List All Rooms\n");
    printf("2. Add a New Room\n");
    printf("3. Search Room / Guest\n");
    printf("4. Generate Daily Report\n");
    printf("5. Save and Exit\n");
    printf("==========================================\n");

    return get_integer("Enter your choice: ", 1, 5);
}


/* Room operations menu */
int room_operations_menu(void)
{
    printf("\n========== ROOM OPERATIONS ==========\n");
    printf("1. View Room Details\n");
    printf("2. Check-In Guest\n");
    printf("3. Check-Out Guest\n");
    printf("4. Change Room Status\n");
    printf("5. Cancel Reservation\n");
    printf("6. Back to Main Menu\n");
    printf("=====================================\n");

    return get_integer("Enter your choice: ", 1, 6);
}


/* Room listing menu */
int room_list_menu(void)
{
    printf("\n========== ROOM LIST ==========\n");
    printf("1. All Rooms\n");
    printf("2. Available Rooms\n");
    printf("3. Occupied Rooms\n");
    printf("4. Maintenance Rooms\n");
    printf("5. Select a Room for Operations\n");
    printf("6. Back to Main Menu\n");
    printf("===============================\n");

    return get_integer("Enter your choice: ", 1, 6);
}


/* Search menu */
int search_menu(void)
{
    printf("\n========== SEARCH MENU ==========\n");
    printf("1. Search by Room Number\n");
    printf("2. Search by Guest Name\n");
    printf("3. Search by Room Type\n");
    printf("4. Search by Room Status\n");
    printf("5. Back to Main Menu\n");
    printf("=================================\n");

    return get_integer("Enter your choice: ", 1, 5);
}


/* Report menu */
int report_menu(void)
{
    printf("\n========== REPORT MENU ==========\n");
    printf("1. Daily Occupancy and Revenue Report\n");
    printf("2. Total Revenue\n");
    printf("3. Back to Main Menu\n");
    printf("================================\n");

    return get_integer("Enter your choice: ", 1, 3);
}


/* Convert room type into readable text */
static const char *get_room_type(RoomType type)
{
    switch (type)
    {
        case ROOM_SINGLE:
            return "Single";

        case ROOM_DOUBLE:
            return "Double";

        case ROOM_SUITE:
            return "Suite";

        default:
            return "Unknown";
    }
}


/* Convert room status into readable text */
static const char *get_room_status(RoomStatus status)
{
    switch (status)
    {
        case ROOM_AVAILABLE:
            return "Available";

        case ROOM_OCCUPIED:
            return "Occupied";

        case ROOM_MAINTENANCE:
            return "Maintenance";

        default:
            return "Unknown";
    }
}


/* Display all rooms */
void display_all_rooms(RoomList *list)
{
    Node *current;

    if (list == NULL || list->head == NULL)
    {
        printf("\nNo rooms available.\n");
        return;
    }

    printf("\n");
    printf("==============================================================\n");
    printf("                       ROOM DETAILS\n");
    printf("==============================================================\n");

    printf("%-10s %-20s %-10s %-12s %-15s\n",
           "Room No.", "Guest Name", "Type",
           "Price", "Status");

    printf("--------------------------------------------------------------\n");

    current = list->head;

    while (current != NULL)
    {
        Room room = current->data;

        printf("%-10d %-20.20s %-10s %-12.2f %-15s\n",
               room.room_number,
               room.guest_name,
               get_room_type(room.type),
               room.price_per_night,
               get_room_status(room.status));

        current = current->next;
    }

    printf("==============================================================\n");
}


/* Display rooms by status */
void display_rooms_by_status(RoomList *list, RoomStatus status)
{
    search_rooms_by_status(list, status);
}


/* Display selected room details */
static void display_room_details(Room *room)
{
    printf("\n========== ROOM DETAILS ==========\n");
    printf("Room Number : %d\n", room->room_number);
    printf("Guest Name  : %s\n", room->guest_name);
    printf("Room Type   : %s\n", get_room_type(room->type));
    printf("Price/Night : %.2f\n", room->price_per_night);
    printf("Nights      : %d\n", room->nights_booked);
    printf("Status      : %s\n", get_room_status(room->status));
    printf("==================================\n");
}


/* Handle search operations */
void handle_search_menu(RoomList *list)
{
    int choice;
    int value;
    char name[50];
    Node *found_room;

    if (list == NULL)
        return;

    do
    {
        choice = search_menu();

        switch (choice)
        {
            case 1:
                value = get_integer("Enter room number: ", 1, 9999);

                if (value == -1)
                    return;

                found_room = search_room_by_number(list, value);

                if (found_room != NULL)
                    display_room_details(&found_room->data);
                else
                    printf("Room not found.\n");

                break;

            case 2:
                if (!get_string("Enter guest name: ",
                                name, sizeof(name)))
                {
                    if (feof(stdin))
                        return;

                    break;
                }

                search_guest_by_name(list, name);
                break;

            case 3:
                printf("\n1. Single\n");
                printf("2. Double\n");
                printf("3. Suite\n");

                value = get_integer("Select room type: ", 1, 3);

                if (value == -1)
                    return;

                search_rooms_by_type(list, (RoomType)(value - 1));
                break;

            case 4:
                printf("\n1. Available\n");
                printf("2. Occupied\n");
                printf("3. Maintenance\n");

                value = get_integer("Select room status: ", 1, 3);

                if (value == -1)
                    return;

                search_rooms_by_status(list, (RoomStatus)(value - 1));
                break;

            case 5:
                printf("Returning to main menu.\n");
                break;

            case -1:
                return;
        }

    } while (choice != 5);
}


/* Handle room listing */
void handle_room_list_menu(RoomList *list)
{
    int choice;

    if (list == NULL)
        return;

    do
    {
        choice = room_list_menu();

        switch (choice)
        {
            case 1:
                display_all_rooms(list);
                break;

            case 2:
                display_rooms_by_status(list, ROOM_AVAILABLE);
                break;

            case 3:
                display_rooms_by_status(list, ROOM_OCCUPIED);
                break;

            case 4:
                display_rooms_by_status(list, ROOM_MAINTENANCE);
                break;

            case 5:
                handle_room_operations(list);
                break;
	   
            case 6:
                break;

            case -1:
                return;
        }

    } while (choice != 6);
}


/* Handle report operations */
void handle_report_menu(RoomList *list)
{
    int choice;

    if (list == NULL)
        return;

    do
    {
        choice = report_menu();

        switch (choice)
        {
            case 1:
                generate_daily_report(list);
                break;

            case 2:
                printf("\nTotal Revenue: %.2f\n",
                       calculate_total_revenue(list));
                break;

            case 3:
                break;

            case -1:
                return;
        }

    } while (choice != 3);
}


/* Handle room operations */
void handle_room_operations(RoomList *list)
{
    int room_number;
    int choice;
    int nights;
    int status_choice;
    char guest_name[50];
    Node *found;
    double bill;

    if (list == NULL)
        return;

    room_number = get_integer("Enter room number: ", 1, 9999);

    if (room_number == -1)
        return;

    found = search_room_by_number(list, room_number);

    if (found == NULL)
    {
        printf("Room not found.\n");
        return;
    }

    do
    {
        choice = room_operations_menu();

        switch (choice)
        {
            case 1:
                display_room_details(&found->data);
                break;

            case 2:
                if (!get_string("Enter guest name: ",
                                guest_name, sizeof(guest_name)))
                {
                    if (feof(stdin))
                        return;
                    break;
                }

                if (guest_name[0] == '\0')
                {
                    printf("Guest name cannot be empty.\n");
                    break;
                }

                nights = get_integer("Enter number of nights: ", 1, 365);

                if (nights == -1)
                    return;

                if (check_in_guest(&found->data, guest_name, nights))
                    printf("Guest checked in successfully!\n");
                else
                    printf("Check-in failed. Room may not be available.\n");

                break;

            case 3:
                bill = check_out_guest(&found->data);

                if (bill >= 0)
                    printf("Guest checked out successfully.\nTotal Bill: %.2f\n",
                           bill);
                else
                    printf("Check-out failed. Room is not occupied.\n");

                break;

            case 4:
                printf("\n1. Available\n");
                printf("2. Maintenance\n");

                status_choice = get_integer("Select new status: ", 1, 2);

                if (status_choice == -1)
                    return;

                if (change_room_status(
                        &found->data,
                        (RoomStatus)(status_choice == 1
                                     ? ROOM_AVAILABLE
                                     : ROOM_MAINTENANCE)))
                {
                    printf("Room status updated successfully.\n");
                }
                else
                {
                    printf("Unable to change room status.\n");
                }

                break;

            case 5:
                if (cancel_reservation(&found->data))
                    printf("Reservation cancelled successfully.\n");
                else
                    printf("Cancellation failed. No occupied reservation found.\n");

                break;

            case 6:
                printf("Returning to main menu.\n");
                break;

            case -1:
                return;
        }

    } while (choice != 6);
}


/* Add a new room */
void handle_add_room(RoomList *list)
{
    Room room;
    int type_choice;
    char price_input[100];
    char *end;
    double price;

    if (list == NULL)
        return;

    printf("\n========== ADD NEW ROOM ==========\n");

    room.room_number = get_integer(
        "Enter room number: ", 1, 9999);

    if (room.room_number == -1)
        return;

    printf("\n1. Single\n");
    printf("2. Double\n");
    printf("3. Suite\n");

    type_choice = get_integer("Select room type: ", 1, 3);

    if (type_choice == -1)
        return;

    room.type = (RoomType)(type_choice - 1);

    if (!get_string("Enter price per night: ",
                    price_input, sizeof(price_input)))
        return;

    errno = 0;
    price = strtod(price_input, &end);

    while (*end == ' ' || *end == '\t')
        end++;

    if (end == price_input || *end != '\0' ||
        errno == ERANGE || price <= 0)
    {
        printf("Invalid room price.\n");
        return;
    }

    room.price_per_night = price;
    strcpy(room.guest_name, "N/A");
    room.nights_booked = 0;
    room.status = ROOM_AVAILABLE;

    if (add_room(list, room))
        printf("\nRoom added successfully!\n");
    else
        printf("\nUnable to add room. Check whether the room number already exists.\n");
}

