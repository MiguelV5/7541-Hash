#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#define CAPACIDAD_MIN_TABLA 3
#define EXITO 0
#define FALLO -1

typedef struct casilla{
    const char* clave;
    void* dato;         // dato == El valor correspondiente a la clave.
    bool esta_vacia;
    bool dato_fue_eliminado;
}casilla_t;

struct hash{
    casilla_t* tabla;
    size_t capacidad;
    size_t cantidad_almacenados;
    hash_destruir_dato_t destructor_dato;
};


/**
 * Función de hash aplicada.
 * Devuelve el valor de hash correspondiente a la clave dada.
 * Para ver más info sobre la operación, ver:
 * README.txt, sección Aclaraciones, "Sobre la 
 * elección de la función de hash".
*/
size_t funcion_de_hash(char* string_clave){
    
    size_t resultado_hash = 5;
    
    while(*string_clave){
        resultado_hash = resultado_hash*3 + *string_clave;
        string_clave++;
    }

    return resultado_hash;
    
}


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){

    hash_t* hash = malloc(1*sizeof(hash_t));
    if(!hash){
        return NULL;
    }

    if(capacidad_inicial <= CAPACIDAD_MIN_TABLA){

        hash->tabla = calloc(CAPACIDAD_MIN_TABLA, sizeof(casilla_t));
        if(!(hash->tabla)){
            free(hash);
            return NULL;
        }
        hash->capacidad = CAPACIDAD_MIN_TABLA;

    }
    else{

        hash->tabla = calloc(capacidad_inicial, sizeof(casilla_t));
        if(!(hash->tabla)){
            free(hash);
            return NULL;
        }
        hash->capacidad = capacidad_inicial;

    }

    hash->destructor_dato = destruir_elemento;
    hash->cantidad_almacenados = 0;

    return hash;

}



int hash_insertar(hash_t* hash, const char* clave, void* elemento){



    return 0;

}




int hash_quitar(hash_t* hash, const char* clave){



    return 0;

}




void* hash_obtener(hash_t* hash, const char* clave){



  return NULL;

}



size_t hash_cantidad(hash_t* hash){



    return 0;

}



bool hash_contiene(hash_t* hash, const char* clave){



    return false;

}


/**
 * Llama al destructor brindado por el usuario por cada dato de la tabla de hash.
*/
void destruir_datos(casilla_t* tabla, size_t tamanio,  hash_destruir_dato_t destructor){

    for(size_t i = 0; i < tamanio; i++){

        if((tabla[i].esta_vacia == false) && (tabla[i].dato_fue_eliminado == false)){
            destructor(tabla[i].dato);
        }

    }

}

void hash_destruir(hash_t* hash){

    if(!hash){
        return;
    }

    if(hash->destructor_dato){
        destruir_datos(hash->tabla, hash->capacidad, hash->destructor_dato);
    }
    free(hash->tabla);
    free(hash);

}



size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){



    return 0;

}

