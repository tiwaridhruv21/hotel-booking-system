hotel: main.o list.o file_io.o booking.o ui.o search.o
	gcc -Wall -Wextra main.o list.o file_io.o booking.o ui.o search.o -o hotel

main.o: main.c
	gcc -Wall -Wextra -c main.c -o main.o

list.o: list.c
	gcc -Wall -Wextra -c list.c -o list.o

file_io.o: file_io.c
	gcc -Wall -Wextra -c file_io.c -o file_io.o

booking.o: booking.c
	gcc -Wall -Wextra -c booking.c -o booking.o

ui.o: ui.c
	gcc -Wall -Wextra -c ui.c -o ui.o

search.o: search.c
	gcc -Wall -Wextra -c search.c -o search.o
