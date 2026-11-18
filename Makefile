CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = main.c list.c persistence.c booking.c ui.c reporting.c
TARGET = hotel

all:
    $(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
    rm -f $(TARGET) $(TARGET).exe *.o
