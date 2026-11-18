hotel: main.o list.o persistence.o booking.o ui.o reporting.o
	gcc -Wall -Wextra main.o list.o persistence.o booking.o ui.o reporting.o -o hotel

main.o: main.c
	gcc -Wall -Wextra -c main.c -o main.o

list.o: list.c
	gcc -Wall -Wextra -c list.c -o list.o

persistence.o: persistence.c
	gcc -Wall -Wextra -c persistence.c -o persistence.o

booking.o: booking.c
	gcc -Wall -Wextra -c booking.c -o booking.o

ui.o: ui.c
	gcc -Wall -Wextra -c ui.c -o ui.o

reporting.o: reporting.c
	gcc -Wall -Wextra -c reporting.c -o reporting.o