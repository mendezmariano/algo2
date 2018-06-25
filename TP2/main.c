#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mensajes.h"//Biblioteca con mensajes a imprimir
#include "tp2.h"


/**************************** CONSTANTES *************************************/
#define ORDENAR "ordenar_archivo"
#define AGREGAR "agregar_archivo"
#define VER "ver_visitantes"
/**************************** FUNCIONES INTERNAS ******************************/
size_t analizar_comando(char* command);
void print_result(bool state,char* command);
/*****************************************************************************/

int main (int argc,char* argv[]){
  if(argc != 2){
    fprintf(stderr, "%s\n",CANT_PARAM_INCORRECTO);
    return ERROR;
  }
  const int max_memory = atoi(argv[1]);
  if(max_memory == 0){
    fprintf(stderr, "%s\n",PARAM_INVALIDO);
      return ERROR;
  }
  char* last_file = NULL;
  bool error_command = false;
  char* buffer = NULL;
  size_t size = 0;
  printf("Ingrese comandos\n");
  while(!error_command && getline(&buffer,&size,stdin)>0 ){
    printf("0\n");
    char** command = split(buffer,' ');
    switch (analizar_comando(command[0])){
      case 1:
        if(last_file)
          free(last_file);
        last_file = strndup(command[2],strlen(command[2])-1);
        error_command = ordenar_archivo(command[1],last_file,max_memory);
        break;
      case 2:
        if(last_file)
          free(last_file);
        last_file = strndup(command[1],strlen(command[1])-1);
        error_command = agregar_archivo(command[1]);
        break;
      case 3:
        printf("0.9");
        error_command = ver_visitantes(last_file,command[1],command[2]);
        break;
      default:
        error_command = true;
        break;
    }
    print_result(error_command,command[0]);
    free_strv(command);
    free(buffer);
    buffer = NULL;
    size = 0;
  }
  if(last_file)
    free(last_file);
  return 0;
}

size_t analizar_comando(char* command){
  size_t selec = 0;
  printf("0.8");
  if( strcmp(command,ORDENAR) == 0)
    selec = 1;
  else if (strcmp(command,AGREGAR)== 0)
    selec = 2;
  else if (strcmp(command,VER) == 0)
    selec = 3;
  return selec;
}
void print_result(bool state,char* command){
  if (state)
    fprintf(stderr, "Error en comando %s\n",command);
  else
    fprintf(stdout,"OK");
}
