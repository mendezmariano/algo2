#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pila.h"

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

typedef struct nodo{
    char* clave;
    void* dato;
    struct nodo* hijo_izq;
    struct nodo* hijo_der;
} nodo_t;

typedef struct abb{
    nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
} abb_t;

// lo tuve que sacar de internet porque el -std=c99 no me dejaba usar strdup
char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);
    if (d == NULL) return NULL;
    strcpy (d,s);
    return d;
}

// Primitivas del abb < > ->, me escribi eso por si queria copiar y pegar

nodo_t* nodo_crear(const char *clave, void *dato){//OK
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo){
        return NULL;
    }
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    nodo->hijo_izq = NULL;
    nodo->hijo_der = NULL;
    return nodo;
}

void* nodo_destruir(abb_t *arbol, nodo_t* nodo){ //OK
    void* dato = nodo->dato;
    if(arbol->destruir_dato){
        arbol->destruir_dato(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
    return dato;
}

//
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){  //OK
    abb_t* arbol = malloc(sizeof(abb_t));
    arbol->raiz = NULL;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    arbol->cantidad = 0;
    return arbol;
}

//
void abb_destruir_recursivo(abb_t *arbol, nodo_t* nodo_actual){//OK
    if(!nodo_actual){
        return;
    }
    abb_destruir_recursivo(arbol, nodo_actual->hijo_izq);
    abb_destruir_recursivo(arbol, nodo_actual->hijo_der);
    nodo_destruir(arbol, nodo_actual);
    return;
}

//
nodo_t* nodo_buscar(const abb_t *arbol, nodo_t* nodo_actual, const char *clave_buscada){//OK
    if(nodo_actual == NULL){
        return NULL;
    }
    int comparacion = arbol->cmp(clave_buscada, nodo_actual->clave);
    if(comparacion < 0){
        return nodo_buscar(arbol, nodo_actual->hijo_izq, clave_buscada);
    }
    if(comparacion > 0){
        return nodo_buscar(arbol, nodo_actual->hijo_der, clave_buscada);
    }
    return nodo_actual;
}

nodo_t* nodo_buscar_padre_recursivo(abb_t *arbol, nodo_t* nodo_actual, const char *clave_buscada, bool* encontro_padre){
    if(nodo_actual == NULL){
        return NULL;
    }

    int comparacion = arbol->cmp(clave_buscada, nodo_actual->clave);
    if(comparacion == 0){
        return nodo_actual;
    }

    nodo_t* nodo_buscado;
    if(comparacion < 0){
        nodo_buscado = nodo_buscar_padre_recursivo(arbol, nodo_actual->hijo_izq, clave_buscada, encontro_padre);
    }
    if(comparacion > 0){
        nodo_buscado = nodo_buscar_padre_recursivo(arbol, nodo_actual->hijo_der, clave_buscada, encontro_padre);
    }

    if(!(*encontro_padre)){
        *encontro_padre = true;
        return nodo_actual;
    }
    return nodo_buscado;
}

//devuelve el padre del nodo con la clave buscada (wrapper)
nodo_t* nodo_buscar_padre(abb_t *arbol, nodo_t* nodo_actual, const char *clave_buscada){ //OK
    bool encontro_padre = false;
    nodo_t* nodo = nodo_buscar_padre_recursivo(arbol, nodo_actual, clave_buscada, &encontro_padre);
    if(!encontro_padre){
        return NULL; //por si el buscado es la raiz
    }
    return nodo;
}

//
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t* padre = nodo_buscar_padre(arbol, arbol->raiz, clave);
    if(!padre){
        arbol->raiz = nodo_crear(clave, dato);
        if(!arbol->raiz){
            return false;
        }
        arbol->cantidad++;
        return true;
    }

    int comparacion = arbol->cmp(clave, padre->clave);
    if(comparacion < 0){
        if(padre->hijo_izq){
            arbol->destruir_dato(padre->hijo_izq->dato);
            padre->hijo_izq->dato = dato;
        } else {
            padre->hijo_izq = nodo_crear(clave, dato);
            if(!padre->hijo_izq){
                return false;
            }
            arbol->cantidad++;
        }
    } else {
        if(padre->hijo_der){
            arbol->destruir_dato(padre->hijo_der->dato);
            padre->hijo_der->dato = dato;
        } else {
            padre->hijo_der = nodo_crear(clave, dato);
            if(!padre->hijo_der){
                return false;
            }
            arbol->cantidad++;
        }
    }
    return true;
}

int obtener_cantidad_de_hijos(nodo_t* nodo){
    int cantidad = 0;
    if(!nodo){
        return 0;
    }

    if(nodo->hijo_izq){
        cantidad++;
    }
    if(nodo->hijo_der){
        cantidad++;
    }
    return cantidad;
}

void* abb_borrar_nodo_sin_hijos(abb_t *arbol, const char *clave, nodo_t* nodo_a_borrar, nodo_t* padre){
    if(!padre){ //borro la raiz
        arbol->raiz = NULL;
        return nodo_destruir(arbol, nodo_a_borrar);
    }

    if(arbol->cmp(clave, padre->hijo_izq->clave) == 0){
        padre->hijo_izq = NULL;
    } else {
        padre->hijo_der = NULL;
    }

    return nodo_destruir(arbol, nodo_a_borrar);
}

void* abb_borrar_nodo_con_1_hijo(abb_t *arbol, const char *clave, nodo_t* nodo_a_borrar, nodo_t* padre){
    if(!padre){ //borro la raiz
        if(nodo_a_borrar->hijo_izq != NULL){
            arbol->raiz = nodo_a_borrar->hijo_izq;
        } else {
            arbol->raiz = nodo_a_borrar->hijo_der;
        }
        return nodo_destruir(arbol, nodo_a_borrar);
    }

    if(nodo_a_borrar->hijo_izq != NULL){
        if(arbol->cmp(clave, padre->hijo_izq->clave) == 0){
            padre->hijo_izq = nodo_a_borrar->hijo_izq;
        } else {
            padre->hijo_der = nodo_a_borrar->hijo_izq;
        }
    } else {
        if(arbol->cmp(clave, padre->hijo_izq->clave) == 0){
            padre->hijo_izq = nodo_a_borrar->hijo_der;
        } else {
            padre->hijo_der = nodo_a_borrar->hijo_der;
        }
    }
    return nodo_destruir(arbol, nodo_a_borrar);
}

nodo_t* buscar_reemplazante(abb_t *arbol, const char *clave, nodo_t* nodo_a_borrar, nodo_t* padre_del_reemplazante){
    nodo_t* nodo_actual = nodo_a_borrar->hijo_der;
    padre_del_reemplazante = nodo_a_borrar;

    while(nodo_actual->hijo_izq){
        padre_del_reemplazante = nodo_actual;
        nodo_actual = nodo_actual->hijo_izq;
    }

    return nodo_actual;
}

void* abb_borrar_nodo_con_2_hijos(abb_t *arbol, const char *clave, nodo_t* nodo_a_borrar, nodo_t* padre){
    nodo_t* padre_del_reemplazante = NULL;
    nodo_t* reemplazante = buscar_reemplazante (arbol, clave, nodo_a_borrar, padre_del_reemplazante);
    //no hay caso donde no exista el reemplazante o su padre

    void* dato_nuevo;
    char* clave_nueva = reemplazante->clave;
    int hijos_del_reemplazante = obtener_cantidad_de_hijos(reemplazante); //para no llamar a abb_borrar y que lo busque de nuevo
    if(hijos_del_reemplazante == 1){
        dato_nuevo = abb_borrar_nodo_con_1_hijo(arbol, clave, reemplazante, padre_del_reemplazante);
    } else {
        dato_nuevo = abb_borrar_nodo_sin_hijos(arbol, clave, reemplazante, padre_del_reemplazante);
    }

    free(nodo_a_borrar->clave);
    void* dato_auxiliar = nodo_a_borrar->dato;

    nodo_a_borrar->clave = clave_nueva;
    nodo_a_borrar->dato = dato_nuevo;

    return dato_auxiliar;
}

//
void *abb_borrar(abb_t *arbol, const char *clave){
    nodo_t* padre = nodo_buscar_padre(arbol, arbol->raiz, clave);
    nodo_t* nodo_a_borrar;
    if(!padre){
        nodo_a_borrar = nodo_buscar(arbol, arbol->raiz, clave);
    } else {
        nodo_a_borrar = nodo_buscar(arbol, padre, clave);
    }
    if(!nodo_a_borrar){
        return NULL;
    }

    arbol->cantidad--;
    int cantidad_de_hijos = obtener_cantidad_de_hijos(nodo_a_borrar);

    if(cantidad_de_hijos == 0){
        return abb_borrar_nodo_sin_hijos(arbol, clave, nodo_a_borrar, padre);
    }
    if (cantidad_de_hijos == 1){
        return abb_borrar_nodo_con_1_hijo(arbol, clave, nodo_a_borrar, padre);
    }
    return abb_borrar_nodo_con_2_hijos(arbol, clave, nodo_a_borrar, padre);
}

//
void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* nodo = nodo_buscar(arbol, arbol->raiz, clave);
    if(nodo == NULL){
        return NULL;
    }

    return nodo->dato;
}

//
bool abb_pertenece(const abb_t *arbol, const char *clave){
    return nodo_buscar(arbol, arbol->raiz, clave) != NULL;
}

//
size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

//
void abb_destruir(abb_t *arbol){
    abb_destruir_recursivo(arbol, arbol->raiz);
}

//iterador interno
void abb_in_order_recursivo(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra, bool* iterar){
    if(!nodo){
        return;
    }
    if(*iterar){
        abb_in_order_recursivo(nodo->hijo_izq, visitar, extra, iterar);
    }
    if(*iterar){
        *iterar = visitar(nodo->clave, nodo->dato, extra);
    }
    if(*iterar){
        abb_in_order_recursivo(nodo->hijo_der, visitar, extra, iterar);
    }
    return;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    bool iterar = true;
    abb_in_order_recursivo(arbol->raiz, visitar, extra, &iterar);
}

//iterador externo

typedef struct abb_iter{
    const abb_t* arbol;
    pila_t* pila;
}abb_iter_t;

void abb_iter_in_destruir(abb_iter_t* iter){
    free(iter->pila);
    free(iter);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter){
        return NULL;
    }
    iter->pila = pila_crear();
    if(!iter->pila){
        free(iter);
        return NULL;
    }

    iter->arbol = arbol;
    nodo_t* nodo_actual = iter->arbol->raiz;
    while(nodo_actual){
        pila_apilar(iter->pila, nodo_actual);
        nodo_actual = nodo_actual->hijo_izq;
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    nodo_t* nodo_actual = pila_desapilar(iter->pila);
    if(!nodo_actual){
        return false;
    }

    nodo_actual = nodo_actual->hijo_der;
    while(nodo_actual){
        pila_apilar(iter->pila, nodo_actual);
        nodo_actual = nodo_actual->hijo_izq;
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    nodo_t* nodo_actual = pila_ver_tope(iter->pila);
    if(!nodo_actual){
        return NULL;
    }

    return nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}
