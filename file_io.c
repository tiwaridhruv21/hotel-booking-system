
#include <stdio.h>
#include "file_io.h"
#include "list.h"

int load_rooms_from_file(RoomList *list)
{
    FILE *file = fopen("rooms.dat", "rb");

    if (file == NULL)
    {
        return 1;
    }

    Room room;
    size_t bytes_read;

    while ((bytes_read = fread(&room, 1, sizeof(Room), file)) > 0)
    {
        if (bytes_read != sizeof(Room))
        {
            fclose(file);
            return 0;
        }

        if (add_room(list, room) == 0)
        {
            fclose(file);
            return 0;
        }
    }

    if (ferror(file))
    {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int save_rooms_to_file(RoomList *list)
{
    FILE *file = fopen("rooms.tmp", "wb");

    if (file == NULL)
    {
        return 0;
    }

    Node *current = list->head;

    while (current != NULL)
    {
        if (fwrite(&current->data, sizeof(Room), 1, file) != 1)
        {
            fclose(file);
            remove("rooms.tmp");
            return 0;
        }

        current = current->next;
    }

    if (fclose(file) != 0)
    {
        remove("rooms.tmp");
        return 0;
    }

    if (rename("rooms.tmp", "rooms.dat") != 0)
    {
        remove("rooms.tmp");
        return 0;
    }

    return 1;
}
