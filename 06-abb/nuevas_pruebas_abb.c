#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <time.h>

int comparacion_entera(const char* a, const char* b){
    return strcmp(a,b);
}
void prueba_abb_vacio(){
  abb_t* abb = abb_crear(comparacion_entera,NULL);
  print_test("* Crear abb",abb);
  print_test("* Cantidad es 0",abb_cantidad(abb) == 0);
  print_test("* Borrar en ABB vacio ",abb_pertenece(abb,"1") == false);
  abb_destruir(abb);
  print_test("* Abb Destruido",!abb);
}

int main()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_abb_vacio();
    //prueba_abb_operaciones();
    //prueba_abb_clave_vacia();
    //prueba_abb_valor_null();
    //prueba_abb_volumen(1, true);
    //prueba_abb_iterar();
    //prueba_abb_iterar_volumen(1);
}
