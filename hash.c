#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#define CAPACIDAD_MIN_TABLA 3
#define CARGA_MAXIMA 0.75
#define EXITO 0
#define FALLO -1

typedef struct casilla{
    char* clave;
    void* dato;         // dato == El valor correspondiente a la clave.
    bool esta_ocupada;
    bool dato_fue_eliminado;
}casilla_t;

struct hash{
    casilla_t* tabla;
    size_t capacidad;
    size_t cantidad_almacenados;
    hash_destruir_dato_t destructor_dato;
};




/**
 * Devuelve un string (reservado en memoria) duplicado del str.
 * Devuelve NULL si falla en reservarlo.
*/
char* duplicar_string(const char* str){

    char* duplicado = malloc((1+strlen(str))*sizeof(char));
    if(!duplicado){
        return NULL;
    }

    strcpy(duplicado, str);
    return duplicado;

}


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




/**
 * Devuelve true si el factor de carga actual de la tabla de hash es mayor o igual que la carga máxima con
 * la cual se debe rehashear.
 * Devuelve false en caso contrario.
*/
bool factor_de_carga_excedido(size_t almacenados, size_t capacidad){

    float carga_actual = almacenados/capacidad;

    return (carga_actual >= CARGA_MAXIMA);

}


/**
 * Reinserta todos los elementos de la tabla de la copia a la tabla de destino.
 * Devuelve 0 si pudo reinsertar todo, -1 en caso contrario.
*/
int reinsercion_tabla(hash_t copia, hash_t* hash_destino){

    size_t i = 0;
    int estado_reinsercion = EXITO;
    while( (i < copia.capacidad) && (estado_reinsercion != FALLO) ){

        if( (copia.tabla[i].esta_ocupada == true) && (copia.tabla[i].dato_fue_eliminado == false) ){

            estado_reinsercion = hash_insertar(hash_destino, copia.tabla[i].clave, copia.tabla[i].dato);
            hash_destino->cantidad_almacenados++;

        }
        i++;

    }
    
    return estado_reinsercion;

}


/**
 * Reemplaza la tabla de hash por una de mayor capacidad y vuelve a insertar cada dato previamente contenido dentro de la misma.
 * Devuelve 0 si pudo rehashear sin problemas, -1 en caso de error.
*/
int rehashear_tabla(hash_t* hash){

    hash_t copia_hash = *hash;

    hash->tabla = calloc(3*(hash->capacidad), sizeof(casilla_t));  //Triplico el tamaño de la tabla (EXPLICAR)
    if(!(hash->tabla)){
        hash->tabla = copia_hash.tabla; //Si falla sale con la misma tabla de antes asi no se pierde la referencia.
        return FALLO;
    }

    hash->capacidad *= 3;
    hash->cantidad_almacenados = 0;

    int resultado_reinsercion = reinsercion_tabla(copia_hash, hash);

    if(resultado_reinsercion == FALLO){
        free(hash->tabla); //Se libera la tabla que se iba a ser el reemplazo pero falló.
        hash->tabla = copia_hash.tabla; //Sale con la misma tabla de antes asi no se pierde la referencia.
        return FALLO;
    }

    free(copia_hash.tabla);
    return EXITO;

}

/**
 * Se encarga de modificar la posición en la que se quería la clave dada para los casos en los que dicha
 * posición estaba ocupada en la tabla.
 * Modifica dicha posición dependiendo del caso que se encuentre al recorrer las casillas:
 * 
 *  - Si se encuentra con la misma clave en la tabla, devuelve la posición en la cual se encuentra la clave
 *    repetida. (Esto a propósito de reemplazar el dato correspondiente a la misma. Esto NO es una colisión).
 * 
 *  - Si se encuentra con una casilla vacía, devuelve la posición de la misma.
 *    (Esto pasa cuando no había una clave repetida, sino cuando hubo una colisión).
 * 
 * (Si la tabla termina, vuelve al inicio de la misma. Por ende se usa el módulo (%) para recorrer).
*/
void buscar_verdadera_posicion_de_insercion(size_t* posicion, casilla_t* tabla, size_t tamanio_tabla, char* clave){

    size_t i = (*posicion);
    bool clave_estaba_repetida = false;

    while((tabla[i%tamanio_tabla].esta_ocupada == true) && (clave_estaba_repetida == false)){

        if(strcmp(tabla[i%tamanio_tabla].clave , clave) == 0){
            clave_estaba_repetida = true;
            (*posicion) = i%tamanio_tabla;
        }
        i++;

    }
    if(clave_estaba_repetida == false){
        (*posicion) = i%tamanio_tabla;
    }

}

/**
 * Llena la casilla de la tabla en la posición dada con la clave y el elemento dado.
*/
void encasillar_en_posicion(size_t posicion, hash_t* hash, char* clave, void* elemento){

    hash->tabla[posicion].clave = clave;
    hash->tabla[posicion].dato = elemento;
    hash->tabla[posicion].esta_ocupada = true;
    hash->tabla[posicion].dato_fue_eliminado = false;
    hash->cantidad_almacenados++;

}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){

    if(!hash || !clave){
        return FALLO;
    }

    if(factor_de_carga_excedido(hash->cantidad_almacenados, hash->capacidad)){
        int resultado_rehasheo = rehashear_tabla(hash);
        if(resultado_rehasheo == FALLO){
            return FALLO;
        }
        else{
            int resultado_insercion = hash_insertar(hash, clave, elemento);
            return resultado_insercion;
        }
    }

    char* clave_duplicada = duplicar_string(clave);
    if(!clave_duplicada){
        return FALLO;
    }

    size_t valor_de_hasheo = funcion_de_hash(clave_duplicada);
    size_t posicion_a_insertar = valor_de_hasheo%(hash->capacidad);

    if(hash->tabla[posicion_a_insertar].esta_ocupada){ //HUBO COLISIÓN O HABÍA CLAVE REPETIDA.

        buscar_verdadera_posicion_de_insercion(&posicion_a_insertar, hash->tabla, hash->capacidad, clave_duplicada);

        encasillar_en_posicion(posicion_a_insertar, hash, clave_duplicada, elemento);
        
    }
    else{

        encasillar_en_posicion(posicion_a_insertar, hash, clave_duplicada, elemento);

    }

    return EXITO;

}




/**
 * Busca en la tabla la clave dada a partir de la posición inicial recibida.
 * Si la encuentra, devuelve la posición en tabla correspondiente a dicha clave.
 * Si no la encuentra, devuelve FALLO (-1).
 * 
 * Deja de buscar cuando encuentra una casilla vacía que no haya tenido antes algo dentro
 * (es decir, que no se haya borrado algo de ahí) o si se dió toda la vuelta a la tabla y 
 * no se encontró la clave.
 * 
 * (Si la tabla termina, vuelve al inicio de la misma. Por ende se usa el módulo (%) para recorrer).
*/
int posicion_buscar_a_partir_de(int posicion_inicial, casilla_t* tabla, size_t tamanio_tabla, char* clave){

    int i = posicion_inicial;

    bool clave_encontrada = false;
    bool se_dio_toda_la_vuelta = false; //Si se llegó a la posición inicial y no se encontró nada.
    int posicion_correspondiente = FALLO;

    while( ((tabla[i%tamanio_tabla].esta_ocupada == true) || (tabla[i%tamanio_tabla].dato_fue_eliminado == false)) && (clave_encontrada == false) && (se_dio_toda_la_vuelta == false)){

        if((tabla[i%tamanio_tabla].esta_ocupada == true) && (tabla[i%tamanio_tabla].dato_fue_eliminado == false)){

            if(strcmp(tabla[i%tamanio_tabla].clave , clave) == 0){
                clave_encontrada = true;
                posicion_correspondiente = (i%tamanio_tabla);
            }

        }

        if( (i%tamanio_tabla) == (posicion_inicial-1) ){
            se_dio_toda_la_vuelta = true;
        }

        i++;

    }
    
    return posicion_correspondiente;

}


/**
 * Llena la casilla de la tabla en la posición dada con los valores correspondientes para completar la eliminación.
 * Si el hash tenía destructor, se lo aplica al dato contenido en la casilla. 
*/
void desencasillar_en_posicion(int posicion, hash_t* hash){

    free(hash->tabla[posicion].clave); //Toda clave insertada era un duplicado.
    hash->tabla[posicion].clave = NULL;
    if(hash->destructor_dato){
        hash->destructor_dato(hash->tabla[posicion].dato);
    }
    hash->tabla[posicion].dato = NULL;
    hash->tabla[posicion].esta_ocupada = false;
    hash->tabla[posicion].dato_fue_eliminado = true;
    hash->cantidad_almacenados--;

}


int hash_quitar(hash_t* hash, const char* clave){

    if(!hash || !clave){
        return FALLO;
    }

    size_t valor_de_hasheo = funcion_de_hash((char*)clave);
    int posicion_en_tabla = valor_de_hasheo%(hash->capacidad);

    int posicion_a_quitar = posicion_buscar_a_partir_de(posicion_en_tabla, hash->tabla, hash->capacidad, (char*)clave);

    if(posicion_a_quitar == FALLO){
        return FALLO;
    }
    
    desencasillar_en_posicion(posicion_a_quitar, hash);
    return EXITO;

}



void* hash_obtener(hash_t* hash, const char* clave){

    if(!hash || !clave){
        return NULL;
    }

    size_t valor_de_hash = funcion_de_hash((char*)clave);
    int posicion = valor_de_hash%(hash->capacidad);

    int posicion_encontrada = posicion_buscar_a_partir_de(posicion, hash->tabla, hash->capacidad, (char*) clave);

    if(posicion_encontrada == FALLO){
        return NULL;
    }
    else{
        return (hash->tabla[posicion_encontrada].dato);
    }

}



size_t hash_cantidad(hash_t* hash){

    if(!hash){
        return 0;
    }

    return hash->cantidad_almacenados;

}




bool hash_contiene(hash_t* hash, const char* clave){

    if(!hash || !clave){
        return false;
    }

    size_t valor_de_hash = funcion_de_hash((char*)clave);
    int posicion = valor_de_hash%(hash->capacidad);

    int posicion_encontrada = posicion_buscar_a_partir_de(posicion, hash->tabla, hash->capacidad, (char*)clave);

    if(posicion_encontrada == FALLO){
        return false;
    }
    else{
        return true;
    }

}


/**
 * Libera las claves de la tabla y llama al destructor brindado por el usuario por cada dato de la tabla de hash
 * (en caso de tenerlo).
 * (La clave se libera debido a que toda clave insertada fue duplicada antes por prevención).
*/
void destruir_casilleros(casilla_t* tabla, size_t tamanio,  hash_destruir_dato_t destructor){

    for(size_t i = 0; i < tamanio; i++){

        if((tabla[i].esta_ocupada == true) && (tabla[i].dato_fue_eliminado == false)){
            
            if(destructor){
                destructor(tabla[i].dato);
            }
            
            free(tabla[i].clave);

        }

    }

}

void hash_destruir(hash_t* hash){

    if(!hash){
        return;
    }
    
    destruir_casilleros(hash->tabla, hash->capacidad, hash->destructor_dato);
    free(hash->tabla);
    free(hash);

}



size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){



    return 0;

}

