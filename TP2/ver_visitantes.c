
bool print_ip(const char * clave, void * dato, void * extra){
    printf(clave);
    return true;
}

bool ver_visitantes(const char* desde, const char* hasta){
    if(!desde || !hasta){
        return false;
    }
    abb_t* abb = abb_crear(ipcmp, NULL);
    if(!abb){
        return false;
    }
    char* buffer = NULL;
    size_t cant = 0;
    char** line;

    char* ip;

    while (getline(&buffer,&cant,input) > 0){
        line = split(buffer,'\t');
        ip = line[0];
        abb_guardar(abb, ip, NULL);
    }
    free_strv(line);
    free(buffer);

    abb_in_order_desde_hasta(abb, print_ip, NULL, desde, hasta);
    abb_destruir(abb);
    return true;
}
