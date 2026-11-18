#include "search.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * Compare two strings without considering uppercase/lowercase.
 *
 * Examples:
 * "John" == "john"
 * "JOHN" == "john"
 * "jOhN" == "John"
 *
 * Returns 0 when the strings are equal.
 */
static int compare_case_insensitive(const char *str1,
                                    const char *str2)
{
    while (*str1 != '\0' && *str2 != '\0') {
        unsigned char c1 = (unsigned char)*str1;
        unsigned char c2 = (unsigned char)*str2;

        if (tolower(c1) != tolower(c2)) {
            return tolower(c1) - tolower(c2);
        }

        str1++;
        str2++;
    }

    return tolower((unsigned char)*str1) -
           tolower((unsigned char)*str2);
}


/*
 * Convert RoomType enum into readable text.
 */
static const char *room_type_to_string(RoomType type)
{
    switch (type) {
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


/*
 * Convert RoomStatus enum into readable text.
 */
static const char *room_status_to_string(RoomStatus status)
{
    switch (status) {
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


/*
 * Search for a room using room number.
 *
 * Returns:
 *     Address of the matching node if found.
 *     NULL if the room does not exist.
 *
 * Time complexity: O(n)
 */
Node *search_room_by_number(RoomList *list, int room_number)
{
    Node *current;

    if (list == NULL) {
        return NULL;
    }

    current = list->head;

    while (current != NULL) {

        if (current->data.room_number == room_number) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}


/*
 * Search for a guest by name.
 *
 * The comparison is CASE-INSENSITIVE.
 *
 * For example:
 *     John
 *     john
 *     JOHN
 *     JoHn
 *
 * will all match the same guest.
 */
int search_guest_by_name(RoomList *list, const char *guest_name)
{
    Node *current;
    int found = 0;

    if (list == NULL ||
        guest_name == NULL ||
        guest_name[0] == '\0') {

        return 0;
    }

    current = list->head;

    printf("\n");
    printf("========================================\n");
    printf("        GUEST SEARCH RESULTS\n");
    printf("========================================\n");

    while (current != NULL) {

        /*
         * Only occupied rooms should have an active guest.
         */
        if (current->data.status == ROOM_OCCUPIED &&
            compare_case_insensitive(
                current->data.guest_name,
                guest_name) == 0) {

            printf("\nRoom Number : %d\n",
                   current->data.room_number);

            printf("Guest Name  : %s\n",
                   current->data.guest_name);

            printf("Room Type   : %s\n",
                   room_type_to_string(current->data.type));

            printf("Price/Night : %.2f\n",
                   current->data.price_per_night);

            printf("Nights      : %d\n",
                   current->data.nights_booked);

            printf("Status      : %s\n",
                   room_status_to_string(current->data.status));

            /*
             * These fields will work if your group adds
             * check_in_time and check_out_time to Room.
             *
             * Uncomment them after your group adds those fields.
             */

            /*
            printf("Check-In    : %s\n",
                   current->data.check_in_time);

            printf("Check-Out   : %s\n",
                   current->data.check_out_time);
            */

            printf("----------------------------------------\n");

            found++;
        }

        current = current->next;
    }

    if (found == 0) {
        printf("No guest found with the name \"%s\".\n",
               guest_name);
    } else {
        printf("Total matches: %d\n", found);
    }

    printf("========================================\n");

    return found;
}


/*
 * Search rooms by room type.
 *
 * Example:
 *     ROOM_SINGLE
 *     ROOM_DOUBLE
 *     ROOM_SUITE
 */
int search_rooms_by_type(RoomList *list, RoomType type)
{
    Node *current;
    int found = 0;

    if (list == NULL) {
        return 0;
    }

    current = list->head;

    printf("\n");
    printf("========================================\n");
    printf("        ROOMS BY TYPE: %s\n",
           room_type_to_string(type));
    printf("========================================\n");

    printf("%-10s %-15s %-15s %-12s\n",
           "Room",
           "Guest",
           "Type",
           "Status");

    printf("----------------------------------------\n");

    while (current != NULL) {

        if (current->data.type == type) {

            printf("%-10d %-15s %-15s %-12s\n",
                   current->data.room_number,
                   current->data.guest_name,
                   room_type_to_string(current->data.type),
                   room_status_to_string(current->data.status));

            found++;
        }

        current = current->next;
    }

    if (found == 0) {
        printf("No rooms found for this room type.\n");
    }

    printf("========================================\n");

    return found;
}


/*
 * Search rooms by status.
 *
 * Example:
 *     ROOM_AVAILABLE
 *     ROOM_OCCUPIED
 *     ROOM_MAINTENANCE
 */
int search_rooms_by_status(RoomList *list, RoomStatus status)
{
    Node *current;
    int found = 0;

    if (list == NULL) {
        return 0;
    }

    current = list->head;

    printf("\n");
    printf("========================================\n");
    printf("        ROOMS BY STATUS: %s\n",
           room_status_to_string(status));
    printf("========================================\n");

    printf("%-10s %-15s %-15s %-12s\n",
           "Room",
           "Guest",
           "Type",
           "Status");

    printf("----------------------------------------\n");

    while (current != NULL) {

        if (current->data.status == status) {

            printf("%-10d %-15s %-15s %-12s\n",
                   current->data.room_number,
                   current->data.guest_name,
                   room_type_to_string(current->data.type),
                   room_status_to_string(current->data.status));

            found++;
        }

        current = current->next;
    }

    if (found == 0) {
        printf("No rooms found for this status.\n");
    }

    printf("========================================\n");

    return found;
}


/*
 * Calculate total revenue.
 *
 * Revenue for an occupied room:
 *
 * price_per_night * nights_booked
 *
 * Example:
 *
 * price = 250
 * nights = 3
 *
 * revenue = 250 * 3 = 750
 */
double calculate_total_revenue(RoomList *list)
{
    Node *current;
    double total_revenue = 0.0;

    if (list == NULL) {
        return 0.0;
    }

    current = list->head;

    while (current != NULL) {

        if (current->data.status == ROOM_OCCUPIED) {

            total_revenue +=
                current->data.price_per_night *
                current->data.nights_booked;
        }

        current = current->next;
    }

    return total_revenue;
}


/*
 * Generate the daily occupancy and revenue report.
 */
void generate_daily_report(RoomList *list)
{
    Node *current;

    int total_rooms = 0;
    int available_rooms = 0;
    int occupied_rooms = 0;
    int maintenance_rooms = 0;

    double total_revenue = 0.0;
    double occupancy_rate = 0.0;

    if (list == NULL) {
        printf("\nUnable to generate report.\n");
        return;
    }

    current = list->head;

    /*
     * Traverse the complete linked list.
     */
    while (current != NULL) {

        total_rooms++;

        switch (current->data.status) {

            case ROOM_AVAILABLE:
                available_rooms++;
                break;

            case ROOM_OCCUPIED:
                occupied_rooms++;

                total_revenue +=
                    current->data.price_per_night *
                    current->data.nights_booked;

                break;

            case ROOM_MAINTENANCE:
                maintenance_rooms++;
                break;

            default:
                break;
        }

        current = current->next;
    }

    /*
     * Calculate occupancy percentage.
     *
     * Cast to double to avoid integer division.
     */
    if (total_rooms > 0) {
        occupancy_rate =
            ((double)occupied_rooms / total_rooms) * 100.0;
    }

    printf("\n");
    printf("============================================\n");
    printf("           HOTEL DAILY REPORT\n");
    printf("============================================\n");

    printf("Total Rooms       : %d\n",
           total_rooms);

    printf("Available Rooms   : %d\n",
           available_rooms);

    printf("Occupied Rooms    : %d\n",
           occupied_rooms);

    printf("Maintenance Rooms : %d\n",
           maintenance_rooms);

    printf("--------------------------------------------\n");

    printf("Occupancy Rate    : %.2f%%\n",
           occupancy_rate);

    printf("Total Revenue     : %.2f\n",
           total_revenue);

    printf("============================================\n");
}