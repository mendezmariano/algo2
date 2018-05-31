CC = gcc
FLAGS = -g -Wall -Wconversion -Wtype-limits -pedantic -Werror
FLAGS_ENTREGA = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VALGRIND = valgrind --leak-check=full --track-origins=yes --show-reachable=yes
GDB = gdbgui
ARCHIVOS_C = nuevas_pruebas_abb.c abb.c testing.c pila.c

cpruebas: $(ARCHIVOS_C)
	$(CC) $(FLAGS) $(ARCHIVOS_C) -o pruebas
	./pruebas

valgrind: pruebas
	$(VALGRIND) ./pruebas

gdb: pruebas
	$(GDB) pruebas

clean:
		rm pruebas
		rm *.o
