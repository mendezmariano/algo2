CC = gcc
FLAGS = -g -Wall -Wconversion -Wtype-limits -pedantic -Werror
FLAGS_ENTREGA = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VALGRIND = valgrind --leak-check=full --track-origins=yes --show-reachable=yes
GDB = gdbgui

cpruebas: pruebas_abb_alumno.c abb.c testing.c pila.c
	$(CC) $(FLAGS) pruebas_abb_alumno.c abb.c testing.c pila.c -o pruebas
	./pruebas

valgrind: pruebas
	$(VALGRIND) ./pruebas

gdb: pruebas
	$(GDB) pruebas
