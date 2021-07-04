#include "hash.h"
#include <stdio.h>
#include "pa2mm.h"

#define EXITO 0
#define FALLO -1

/**
 * Función auxiliar de prueba.
 * Libera el puntero recibido.
*/
void destructor_de_prueba(void* dato){

  free(dato);

}


// PRUEBAS DE CREACIÓN (1)

void DadoDestructorNullODestructorNoNull_SiSePideCrear_SeObtieneHashNoNull(){

  hash_t* hash = NULL;
  hash = hash_crear(destructor_de_prueba, 5);
  pa2m_afirmar( hash != NULL , "Se creó un hash con destructor.");
  hash_destruir(hash);

  hash = hash_crear(NULL, 5);
  pa2m_afirmar( hash != NULL , "Se creó un hash sin destructor.");
  hash_destruir(hash);

  printf("\n");

}


void DadaCapacidadInicialCualquiera_SiSePideCrear_SeObtieneHashNoNull(){

  hash_t* hash = NULL;
  hash = hash_crear(destructor_de_prueba, 100);
  pa2m_afirmar( hash != NULL , "Se creó un hash con capacidad mayor a 3.");
  hash_destruir(hash);

  hash = hash_crear(destructor_de_prueba, 0);
  pa2m_afirmar( hash != NULL , "Se creó un hash con capacidad 3 pidiendose una capacidad menor."); //Si bien no se puede acceder a la estructura para verificar realmente la capacidad, se verifica la existencia del hash.
  hash_destruir(hash);

  hash = hash_crear(destructor_de_prueba, 1);
  pa2m_afirmar( hash != NULL , "Se creó otro hash con capacidad 3 pidiendose capacidad menor.");
  hash_destruir(hash);

  printf("\n");

}


// PRUEBAS DE INSERCIÓN / CANTIDAD DE ALMACENADOS / CONTENCIÓN


void DadoHashInexistenteOConTablaVacia_SiSePideCantidadDeAlmacenados_SeDevuelveCero(){

  pa2m_afirmar( hash_cantidad(NULL) == 0 , "Un hash inexistente tiene cero como cantidad de elementos");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_cantidad(hash) == 0 , "Un hash con tabla vacía tiene cero como cantidad de elementos");
  hash_destruir(hash);

  printf("\n");

}




void DadaTablaConDatos_SiSePideCantidadDeAlmacenados_SeDevuelveCantidadCorrecta(){

  hash_t* hash = hash_crear(NULL, 10);
  int uno = 1 , dos = 2 , tres = 3; //Datos de prueba.

  hash_insertar(hash, "uno", &uno);
  hash_insertar(hash, "dos", &dos);
  hash_insertar(hash, "tres", &tres);

  pa2m_afirmar( hash_cantidad(hash) == 3 , "Pedir la cantidad de almacenados de un hash con varios datos devuelve el valor adecuado.");

  hash_destruir(hash);

  printf("\n");

}


//


void DadoHashInexistenteOClaveNull_SiSePideInsertar_SeDevuelveFallo(){

  char* clave_prueba = "holaclave"; 
  char* dato_prueba = "holadato";
  pa2m_afirmar( hash_insertar(NULL, clave_prueba, dato_prueba) == FALLO , "No se puede insertar en hash inexistente.");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_insertar(hash, NULL, dato_prueba) == FALLO , "No se puede insertar un dato con clave NULL.");
  hash_destruir(hash);

  printf("\n");

}


void DadaTablaVaciaDeGranCapacidad_SiSeInsertanVariosDatos_SeInsertanCorrectamente(){

  hash_t* hash = hash_crear(NULL, 30);
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int uno = 1 , dos = 2 , tres = 3 , cuatro = 4 ; //Datos de prueba.

  pa2m_afirmar( hash_insertar(hash, clave_uno, &uno) == EXITO , "Se insertó un dato en una tabla vacía.");
  pa2m_afirmar( hash_contiene(hash, clave_uno) == true , "El dato insertado está contenido en la tabla.");

  pa2m_afirmar( hash_insertar(hash, clave_dos, &dos) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_dos) == true , "El dato insertado está contenido en la tabla.");

  pa2m_afirmar( hash_insertar(hash, clave_tres, &tres) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_tres) == true , "El dato insertado está contenido en la tabla.");

  pa2m_afirmar( hash_insertar(hash, clave_cuatro, &cuatro) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_cuatro) == true , "El dato insertado está contenido en la tabla.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaPropensaARehashear_SiSeInsertaDato_SeInsertaCorrectamenteTrasRehashear(){



  printf("\n");

}


//

void DadoHashInexistenteOClaveNullOTablaVacia_SiSePideVerificarContencion_SeDevuelveFalse(){



  printf("\n");

}


void DadaTablaConDatos_SiSePideVerificarContencionDeDatoNoContenido_SeDevuelveFalse(){



  printf("\n");

}


void DadaTablaConDatos_SiSeVerificaContencionDeDatoContenido_SeDevuelveTrue(){



  printf("\n");

}





int main(){

  pa2m_nuevo_grupo("Pruebas de creación (1)");
    DadoDestructorNullODestructorNoNull_SiSePideCrear_SeObtieneHashNoNull();
    DadaCapacidadInicialCualquiera_SiSePideCrear_SeObtieneHashNoNull();

  pa2m_nuevo_grupo("Pruebas de inserción, cantidad de almacenados y contención"); // Se usan las tres funciones para probarse entre sí, son muy dependientes la una de la otra.
    DadoHashInexistenteOConTablaVacia_SiSePideCantidadDeAlmacenados_SeDevuelveCero();
    DadaTablaConDatos_SiSePideCantidadDeAlmacenados_SeDevuelveCantidadCorrecta();

    DadoHashInexistenteOClaveNull_SiSePideInsertar_SeDevuelveFallo();
    DadaTablaVaciaDeGranCapacidad_SiSeInsertanVariosDatos_SeInsertanCorrectamente(); //Para asegurar que no rehashee
    DadaTablaPropensaARehashear_SiSeInsertaDato_SeInsertaCorrectamenteTrasRehashear(); //Se asegura rehash

    DadoHashInexistenteOClaveNullOTablaVacia_SiSePideVerificarContencion_SeDevuelveFalse();
    DadaTablaConDatos_SiSePideVerificarContencionDeDatoNoContenido_SeDevuelveFalse();
    DadaTablaConDatos_SiSeVerificaContencionDeDatoContenido_SeDevuelveTrue();

    //elemento esta despues de rehash
/*
  pa2m_nuevo_grupo("Pruebas de ");
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();

  pa2m_nuevo_grupo("Pruebas de ");
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();

  pa2m_nuevo_grupo("Pruebas de ");
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();

  pa2m_nuevo_grupo("Pruebas de ");
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();

  pa2m_nuevo_grupo("Pruebas de ");
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
    Dado_Si_X();
*/


  return pa2m_mostrar_reporte();
}
