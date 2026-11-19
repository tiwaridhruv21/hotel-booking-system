#include <stdio.h>
#include "room.h"
#include "list.h"
#include "file_io.h"
#include "ui.h"

int main(void) {
    RoomList list;
    init_room_list(&list);

    load_rooms_from_file(&list);
    main_menu(&list);

    if (save_rooms_to_file(&list)) {
        printf("Data saved successfully.\n");
    } else {
        printf("Warning: Failed to save data.\n");
    }

    free_room_list(&list);
    return 0;
}
