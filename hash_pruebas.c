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


// PRUEBAS DE CREACIÓN 

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


// PRUEBAS DE CANTIDAD DE ALMACENADOS / INSERCIÓN / CONTENCIÓN

// (Tendiendo a cantidad):

void DadoHashInexistenteOTablaVacia_SiSePideCantidadDeAlmacenados_SeDevuelveCero(){

  pa2m_afirmar( hash_cantidad(NULL) == 0 , "Un hash inexistente tiene cero como cantidad de elementos");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_cantidad(hash) == 0 , "Un hash con tabla vacía tiene cero como cantidad de elementos");
  hash_destruir(hash);

  printf("\n");

}




void DadaTablaConDatos_SiSePideCantidadDeAlmacenados_SeDevuelveCantidadCorrecta(){

  hash_t* hash = hash_crear(NULL, 10);
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres"; //Claves de prueba.
  int uno = 1 , dos = 2 , tres = 3; //Datos de prueba.

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);

  pa2m_afirmar( hash_cantidad(hash) == 3 , "Pedir la cantidad de almacenados de un hash con varios datos devuelve el valor adecuado.");

  hash_destruir(hash);

  printf("\n");

}


// (Tendiendo a Inserción):


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
  pa2m_afirmar( hash_cantidad(hash) == 1 , "La cantidad de datos almacenados es 1.");

  pa2m_afirmar( hash_insertar(hash, clave_dos, &dos) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_dos) == true , "El dato insertado está contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 2 , "La cantidad de datos almacenados es 2.");

  pa2m_afirmar( hash_insertar(hash, clave_tres, &tres) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_tres) == true , "El dato insertado está contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 3 , "La cantidad de datos almacenados es 3.");

  pa2m_afirmar( hash_insertar(hash, clave_cuatro, &cuatro) == EXITO , "Se insertó otro dato en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_cuatro) == true , "El dato insertado está contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 4 , "La cantidad de datos almacenados es 4.");

  pa2m_afirmar( hash_insertar(hash, clave_cuatro, &uno) == EXITO , "Se intenta insertar una clave repetida en la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_cuatro) == true , "El dato correspondiente a la misma clave sigue contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 4 , "La cantidad de datos almacenados sigue siendo 4.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaPropensaARehashear_SiSeInsertaDato_SeInsertaCorrectamenteTrasRehashear(){

  //Creo una tabla de capacidad 4 y la lleno con 3 datos. Tras la 3ra insercion el factor de carga se vuelve 0.75,
  //con lo cual recién al tratar de insertar un 4to dato se va a rehashear primero y luego insertar ese dato.

  hash_t* hash = hash_crear(NULL, 4);

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int uno = 1 , dos = 2 , tres = 3 , cuatro = 4 ; //Datos de prueba.

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_dos, &dos);

  pa2m_afirmar( hash_insertar(hash, clave_cuatro, &cuatro) == EXITO , "Se insertó un dato cuando la tabla necesitaba rehashear");
  pa2m_afirmar( hash_contiene(hash, clave_cuatro) == true , "El dato insertado está contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 4 , "La cantidad de datos almacenados es la requerida tras el rehash.");
  
  hash_destruir(hash);
  printf("\n");

}


// (Tendiendo a contención):

void DadoHashInexistenteOClaveNullOTablaVacia_SiSePideVerificarContencion_SeDevuelveFalse(){

  char* clave_prueba = "holaclave"; 
  pa2m_afirmar( hash_contiene(NULL, clave_prueba) == false, "Un hash inexistente no contiene ninguna clave a verificar.");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_contiene(hash, NULL) == false, "Un hash no puede contener una clave inexistente.");
  pa2m_afirmar( hash_contiene(hash, clave_prueba) == false, "Una tabla vacía no contiene ninguna clave.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaConDatos_SiSePideVerificarContencionDeDatoNoContenido_SeDevuelveFalse(){

  hash_t* hash = hash_crear(NULL, 10);
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int uno = 1 , dos = 2 , tres = 3; //Datos de prueba.

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);

  pa2m_afirmar( hash_contiene(hash, clave_cuatro) == false , "Verificar contencion de una clave que no existe en la tabla devuelve false.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaConDatos_SiSeVerificaContencionDeDatoContenido_SeDevuelveTrue(){

  hash_t* hash = hash_crear(NULL, 10);
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int uno = 1 , dos = 2 , tres = 3 , cuatro = 4 ; //Datos de prueba.

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_cuatro, &cuatro);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_dos, &dos);

  pa2m_afirmar( (hash_contiene(hash, clave_uno) == true) && 
  (hash_contiene(hash, clave_dos) == true) && 
  (hash_contiene(hash, clave_tres) == true ) && 
  (hash_contiene(hash, clave_cuatro) == true) , 
  "Verificar contencion de todas las claves pertenecientes a una tabla devuelve true.");

  hash_destruir(hash);
  printf("\n");

}



// PRUEBAS DE DESTRUCCION DE HASH

void DadoHashConDatosReservadosEnHeap_SiSePasaDestructorDePrueba_SeLiberanTodosLosDatosAlDestruir(){

  hash_t* hash = hash_crear(destructor_de_prueba, 10);

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  
  int* uno = malloc(1*sizeof(int)); //Datos alojados de prueba.
  if(!uno){
    return;
  }
  *uno = 1;

  int* dos = malloc(1*sizeof(int)); 
  if(!dos){
    free(uno);
    return;
  }
  *dos = 2;

  int* tres = malloc(1*sizeof(int)); 
  if(!tres){
    free(uno);
    free(dos);
    return;
  }
  *tres = 3;


  hash_insertar(hash, clave_tres, tres);
  hash_insertar(hash, clave_uno, uno);
  hash_insertar(hash, clave_dos, dos);

  hash_destruir(hash); //Si pasa de acá sin errores de memoria es porque se liberó todo correctamente.
  pa2m_afirmar( true , "Se destruyó correctamente el hash.");

  printf("\n");

}


// PRUEBAS DE OBTENCION

void DadoHashInexistenteOClaveNullOTablaVacia_SiSePideObtenerUnDato_SeDevuelveNull(){

  char* clave_prueba = "holaclave"; 
  pa2m_afirmar( hash_obtener(NULL, clave_prueba) == NULL, "No se pueden obtener datos de un hash inexistente.");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_obtener(hash, NULL) == NULL, "No se puede obtener un dato con una clave inexistente.");
  pa2m_afirmar( hash_obtener(hash, clave_prueba) == NULL, "No se puede obtener un dato de una tabla vacía.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaConDatos_SiSePideObtenerUnDatoDatoQueNoEstaEnLaTabla_SeDevuelveNull(){

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int dos = 2 , tres = 3, cuatro = 4; //Datos de prueba.

  hash_t* hash = hash_crear(NULL, 10);

  hash_insertar(hash, clave_cuatro, &cuatro);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_dos, &dos);

  pa2m_afirmar( hash_obtener(hash, clave_uno) == NULL , "Al intentar obtener un dato que no está en una tabla se devuelve NULL.");

  hash_destruir(hash);
  printf("\n");

}


void DadaTablaConDatos_SiSePideObtenerUnDatoQueEstaEnLaTabla_SeDevuelveElDatoCorrectamente(){

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres"; //Claves de prueba.
  int uno = 1, dos = 2 , tres = 3; //Datos de prueba.

  hash_t* hash = hash_crear(NULL, 10);

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_dos, &dos);

  pa2m_afirmar( *(int*)hash_obtener(hash, clave_uno) == 1 , "Obtener un dato que está en la tabla resulta en el dato correcto (1).");
  pa2m_afirmar( *(int*)hash_obtener(hash, clave_dos) == 2 , "Obtener un dato que está en la tabla resulta en el dato correcto (2).");
  pa2m_afirmar( *(int*)hash_obtener(hash, clave_tres) == 3 , "Obtener un dato que está en la tabla resulta en el dato correcto (3).");

  hash_destruir(hash);
  printf("\n");

}




// PRUEBAS DE QUITAR DATOS


void DadoHashInexistenteOClaveNullOTablaVacia_SiSePideQuitarUnDato_SeDevuelveFallo(){

  char* clave_prueba = "holaclave"; 
  pa2m_afirmar( hash_quitar(NULL, clave_prueba) == FALLO, "No se pueden quitar datos de un hash inexistente.");

  hash_t* hash = hash_crear(NULL, 3);
  pa2m_afirmar( hash_quitar(hash, NULL) == FALLO, "No se puede quitar un dato con una clave inexistente.");
  pa2m_afirmar( hash_quitar(hash, clave_prueba) == FALLO, "No se puede quitar un dato de una tabla vacía.");

  hash_destruir(hash);
  printf("\n");

}



void DadaTablaConDatos_SiSeIntentaQuitarDatoQueNoEstaEnTabla_SeDevuelveFallo(){

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; //Claves de prueba.
  int uno = 1, dos = 2, cuatro = 4; //Datos de prueba.

  hash_t* hash = hash_crear(NULL, 10);

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_cuatro, &cuatro);

  pa2m_afirmar( hash_quitar(hash, clave_tres) == FALLO , "Al intentar quitar un dato que no está en una tabla se devuelve FALLO.");

  hash_destruir(hash);
  printf("\n");

}



void DadaTablaConDatos_SiSeQuitanDatosQueEstanEnLaTabla_SeQuitanCorrectamente(){

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres"; //Claves de prueba.
  int uno = 1, dos = 2 , tres = 3; //Datos de prueba.

  hash_t* hash = hash_crear(NULL, 10);

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);

  pa2m_afirmar( hash_quitar(hash, clave_uno) == EXITO , "Se quitó un dato de una tabla con 3 datos.");
  pa2m_afirmar( hash_contiene(hash, clave_uno) == false , "La tabla ya no contiene al dato recién quitado.");
  pa2m_afirmar( hash_cantidad(hash) == 2 , "La cantidad de datos almacenados en la tabla ahora es 2.");  

  pa2m_afirmar( hash_quitar(hash, clave_dos) == EXITO , "Se quitó otro dato de la tabla.");
  pa2m_afirmar( hash_contiene(hash, clave_dos) == false , "La tabla ya no contiene al dato recién quitado.");
  pa2m_afirmar( hash_cantidad(hash) == 1 , "La cantidad de datos almacenados en la tabla ahora es 1.");  

  hash_destruir(hash);
  printf("\n");

  // Se prueba borrado con destructor de dato:
  hash = hash_crear(destructor_de_prueba, 10);

  int* uno_alloc = malloc(1*sizeof(int)); //Datos alojados de prueba.
  if(!uno_alloc){
    return;
  }
  *uno_alloc = 1;

  int* dos_alloc = malloc(1*sizeof(int)); 
  if(!dos_alloc){
    free(uno_alloc);
    return;
  }
  *dos_alloc = 2;

  int* tres_alloc = malloc(1*sizeof(int)); 
  if(!tres_alloc){
    free(uno_alloc);
    free(dos_alloc);
    return;
  }
  *tres_alloc = 3;

  hash_insertar(hash, clave_uno, uno_alloc);
  hash_insertar(hash, clave_dos, dos_alloc);
  hash_insertar(hash, clave_tres, tres_alloc);

  pa2m_afirmar( hash_quitar(hash, clave_uno) == EXITO , "Se quitó un dato de una tabla (con destructor) con 3 datos.");
  // Alcanza con ver si falla valgrind para saber si se destruyó el dato o no. Por ende no hay afirmación sobre eso.
  pa2m_afirmar( hash_contiene(hash, clave_uno) == false , "La tabla ya no contiene al dato recién quitado.");
  pa2m_afirmar( hash_cantidad(hash) == 2 , "La cantidad de datos almacenados en la tabla ahora es 2.");  

  hash_destruir(hash);

  printf("\n");

}



void DadaTablaConDatos_SiSeQuitanTodosLosDatos_ResultaTablaVaciaYSePuedenVolverAInsertarTodos(){

  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres"; //Claves de prueba.
  int uno = 1, dos = 2 , tres = 3; //Datos de prueba.

  hash_t* hash = hash_crear(NULL, 4);

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);

  hash_quitar(hash, clave_tres);
  hash_quitar(hash, clave_dos);
  hash_quitar(hash, clave_uno);

  pa2m_afirmar( hash_cantidad(hash) == 0 , "Se quitaron todos los datos de una tabla, la cantidad de almacenados es cero.");


  pa2m_afirmar( hash_insertar(hash, clave_uno, &uno) == EXITO , "Se pudo volver a insertar un dato de los que se borraron antes.");
  pa2m_afirmar( hash_contiene(hash, clave_uno) == true , "El dato insertado está nuevamente contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 1 , "La cantidad de datos almacenados es 1.");


  pa2m_afirmar( hash_insertar(hash, clave_dos, &dos) == EXITO , "Se pudo volver a insertar otro dato.");
  pa2m_afirmar( hash_contiene(hash, clave_dos) == true , "El dato insertado está nuevamente contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 2 , "La cantidad de datos almacenados es 2.");


  pa2m_afirmar( hash_insertar(hash, clave_tres, &tres) == EXITO , "Se pudo volver a insertar otro dato.");
  pa2m_afirmar( hash_contiene(hash, clave_tres) == true , "El dato insertado está nuevamente contenido en la tabla.");
  pa2m_afirmar( hash_cantidad(hash) == 3 , "La cantidad de datos almacenados es 3.");


  hash_destruir(hash);
  printf("\n");

}


// PRUEBAS DE ITERADOR


// ============================================== / / ============================================== //

/**
 * Función auxiliar de prueba. PARA USAR EN PRUEBA DE ITERADOR SIN CORTE
 * Le suma 'aumento' al contenido del dato correspondiente a la clave.
 * Siempre devuelve false
*/
bool sin_corte_aumento_y_muestreo(hash_t* hash, const char* clave, void* aumento){

  int* entero_correspondiente = hash_obtener(hash, clave);
  (*entero_correspondiente) += (*(int*)aumento);

  return false;

}

/**
 * Función auxiliar de prueba. PARA USAR EN PRUEBA DE ITERADOR CON CORTE
 * Le suma 'aumento' al contenido del dato correspondiente a la clave.
 * Devuelve true siempre (Entonces solo debería aplicarse una vez en el iterador).
*/
bool con_corte_aumento_y_muestreo(hash_t* hash, const char* clave, void* aumento){

  sin_corte_aumento_y_muestreo(hash, clave, aumento);
  return true;

}


// ============================================== / / ============================================== //



void DadoHashOFuncionInexistentesOTablaVacia_SiSeIntentaUsarIterador_SeDevuelveCero(){

  int aumento_de_prueba = 1;
  pa2m_afirmar(hash_con_cada_clave(NULL, sin_corte_aumento_y_muestreo, &aumento_de_prueba) == 0, "No se puede usar iterador con un hash inexistente.");

  hash_t* hash = hash_crear(NULL, 10);
  pa2m_afirmar(hash_con_cada_clave(hash, NULL, &aumento_de_prueba) == 0, "No se puede usar iterador con una función inexistente.");

  pa2m_afirmar(hash_con_cada_clave(hash, sin_corte_aumento_y_muestreo, &aumento_de_prueba) == 0, "Iterar en una tabla vacía devuelve cero.");  
  
  hash_destruir(hash);
  printf("\n");

} 



void DadaTablaConDatos_SiSeUsaIteradorSinCorte_SeIteranTodosLosDatos(){
  
  hash_t* hash = hash_crear(NULL, 10);
  int aumento_de_prueba = 1;
  
  int uno = 1, dos = 2, tres = 3, cuatro = 4; //Elementos de prueba
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; // Claves de prueba

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_cuatro, &cuatro);

  pa2m_afirmar( hash_con_cada_clave(hash, sin_corte_aumento_y_muestreo, &aumento_de_prueba) == 4, "Se iteran todos los datos de una tabla. (iterador sin cortes)");
  pa2m_afirmar( uno == 2 && dos == 3 && tres == 4 && cuatro == 5 , "La función pasada se aplicó correctamente a todos los datos.");

  hash_destruir(hash);
  printf("\n");

} 



void DadaTablaConDatos_SiSeUsaIteradorConCorte_SeIteranSoloLosDatosRequeridos(){

  
  hash_t* hash = hash_crear(NULL, 10);
  int aumento_de_prueba = 1;
  
  int uno = 1, dos = 2, tres = 3, cuatro = 4; //Elementos de prueba
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  char* clave_cuatro = "cuatro"; // Claves de prueba

  hash_insertar(hash, clave_uno, &uno);
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_tres, &tres);
  hash_insertar(hash, clave_cuatro, &cuatro);

  pa2m_afirmar( hash_con_cada_clave(hash, con_corte_aumento_y_muestreo, &aumento_de_prueba) == 1, "Se iteran solo los datos necesarios de una tabla. (iterador con corte)");
  
  hash_destruir(hash);
  printf("\n");

} 





// PRUEBAS ADICIONALES

void DadaTablaDeCapacidadMinima_SiSeObligaColisionForzadaYSeBorraCasiTodo_NoSePuedeInsertarClaveRepetida(){

  hash_t* hash = hash_crear(NULL, 3);
  char* clave_uno = "uno";
  char* clave_dos = "dos";
  char* clave_tres = "tres";
  int uno = 1 , dos = 2 , tres = 3 ; //Datos de prueba.

  
  hash_insertar(hash, clave_dos, &dos);
  hash_insertar(hash, clave_uno, &uno);
  hash_quitar(hash, clave_dos);


  hash_insertar(hash, clave_tres, &tres);
  hash_quitar(hash, clave_tres);

  hash_insertar(hash, clave_uno, &uno);

  pa2m_afirmar( hash_cantidad(hash) == 1 , "Colisionan dos inserciones, se borra un dato, se colisiona otro y se quita, al intentar insertar el repetido que sigue en la tabla NO se puede insertar (caso extremo de uso de flags)."); // (Nuevamente) Leer aclaraciones.

  hash_destruir(hash);
  printf("\n");

}


void DadoUnDestructorNoNullYTablaConDatos_SiSeInsertanClavesRepetidas_LosDatosAntiguosSonCorrectamenteManejadosAlReemplazar(){
  //

  hash_t* hash = hash_crear(destructor_de_prueba, 10);

  char* clave_uno = "uno";
  
  int* uno = malloc(1*sizeof(int)); //Datos alojados de prueba.
  if(!uno){
    return;
  }
  *uno = 1;

  int* uno_repetido = uno; 

  int* tres = malloc(1*sizeof(int)); 
  if(!tres){
    free(uno);
    free(uno_repetido);
    return;
  }
  *tres = 3;


  hash_insertar(hash, clave_uno, uno); //Se inserta dato.
  hash_insertar(hash, clave_uno, uno_repetido); //Se inserta misma clave CON MISMO DATO. No debería destruir el dato antiguo porque es el mismo de antes.
  hash_insertar(hash, clave_uno, tres);//Se inserta misma clave CON  DISTINTO DATO. Debería destruir el dato antiguo porque es distinto al nuevo.

  //Si no hubo errores de memoria en valgrind entonces estuvo todo correcto.
  pa2m_afirmar( hash_cantidad(hash) == 1 , "El manejo de inserción de claves repetidas (con o sin dato repetido) del hash es el adecuado.");

  printf("\n");

  hash_destruir(hash); 

}

void DadaTabla_SiSeRehasheaMasDeUnaVez_TodosLosElementosSeInsertanCorrectamente(){

  hash_t* hash = hash_crear(NULL, 3);
   

  char* datos_de_prueba[10] = {"holadato", "holadato2" , "aguantegdb", "pa2m_afirmar" , "123456", "otadaloh" , "2otadaloh" , "aksdjkjad" , "765213" , "wasd"};
  char* claves_prueba[10] = {"c1", "c2" , "c3" , "c4" , "c5", "c6", "c7", "c8" , "c9" , "c10"};

  hash_insertar(hash, claves_prueba[0], datos_de_prueba[0]);
  hash_insertar(hash, claves_prueba[1], datos_de_prueba[1]);
  hash_insertar(hash, claves_prueba[2], datos_de_prueba[2]); //Primer rehash: aumenta capacidad a 9.
  
  hash_insertar(hash, claves_prueba[3], datos_de_prueba[3]);
  hash_insertar(hash, claves_prueba[4], datos_de_prueba[4]);
  hash_insertar(hash, claves_prueba[5], datos_de_prueba[5]);
  hash_insertar(hash, claves_prueba[6], datos_de_prueba[6]);
  
  pa2m_afirmar(hash_insertar(hash, claves_prueba[7], datos_de_prueba[7]) == EXITO , "Se inserta dato cuando se rehashea por segunda vez."); //Segundo rehash: aumenta capacidad a 27.
  
  hash_insertar(hash, claves_prueba[8], datos_de_prueba[8]);
  hash_insertar(hash, claves_prueba[9], datos_de_prueba[9]);
  
  pa2m_afirmar( hash_obtener(hash, claves_prueba[7]) == datos_de_prueba[7] , "El dato insertado tras dicho rehash es el adecuado.");
  pa2m_afirmar( hash_cantidad(hash) == 10 , "La cantidad de elementos almacenados en el hash es la correcta.");

  hash_destruir(hash);
  printf("\n");

}




int main(){

  pa2m_nuevo_grupo("Pruebas de creación");
    DadoDestructorNullODestructorNoNull_SiSePideCrear_SeObtieneHashNoNull();
    DadaCapacidadInicialCualquiera_SiSePideCrear_SeObtieneHashNoNull();

  pa2m_nuevo_grupo("Pruebas de cantidad de almacenados, inserción y contención"); // Se usan las tres funciones para probarse entre sí, son muy dependientes la una de la otra. Ver Aclaraciones.
    DadoHashInexistenteOTablaVacia_SiSePideCantidadDeAlmacenados_SeDevuelveCero();
    DadaTablaConDatos_SiSePideCantidadDeAlmacenados_SeDevuelveCantidadCorrecta();

    DadoHashInexistenteOClaveNull_SiSePideInsertar_SeDevuelveFallo();
    DadaTablaVaciaDeGranCapacidad_SiSeInsertanVariosDatos_SeInsertanCorrectamente(); //Para asegurar que no rehashee
    DadaTablaPropensaARehashear_SiSeInsertaDato_SeInsertaCorrectamenteTrasRehashear(); //Se asegura rehash

    DadoHashInexistenteOClaveNullOTablaVacia_SiSePideVerificarContencion_SeDevuelveFalse();
    DadaTablaConDatos_SiSePideVerificarContencionDeDatoNoContenido_SeDevuelveFalse();
    DadaTablaConDatos_SiSeVerificaContencionDeDatoContenido_SeDevuelveTrue();

  pa2m_nuevo_grupo("Pruebas de destrucción de hash"); //Recién acá la hago porque se necesita poder insertar.
    DadoHashConDatosReservadosEnHeap_SiSePasaDestructorDePrueba_SeLiberanTodosLosDatosAlDestruir(); 

  pa2m_nuevo_grupo("Pruebas de obtención");
    DadoHashInexistenteOClaveNullOTablaVacia_SiSePideObtenerUnDato_SeDevuelveNull();
    DadaTablaConDatos_SiSePideObtenerUnDatoDatoQueNoEstaEnLaTabla_SeDevuelveNull();
    DadaTablaConDatos_SiSePideObtenerUnDatoQueEstaEnLaTabla_SeDevuelveElDatoCorrectamente();

  pa2m_nuevo_grupo("Pruebas de quitar");
    DadoHashInexistenteOClaveNullOTablaVacia_SiSePideQuitarUnDato_SeDevuelveFallo();
    DadaTablaConDatos_SiSeIntentaQuitarDatoQueNoEstaEnTabla_SeDevuelveFallo();
    DadaTablaConDatos_SiSeQuitanDatosQueEstanEnLaTabla_SeQuitanCorrectamente();
    DadaTablaConDatos_SiSeQuitanTodosLosDatos_ResultaTablaVaciaYSePuedenVolverAInsertarTodos();

  pa2m_nuevo_grupo("Pruebas de iterador");
    DadoHashOFuncionInexistentesOTablaVacia_SiSeIntentaUsarIterador_SeDevuelveCero();
    DadaTablaConDatos_SiSeUsaIteradorSinCorte_SeIteranTodosLosDatos();
    DadaTablaConDatos_SiSeUsaIteradorConCorte_SeIteranSoloLosDatosRequeridos();

  pa2m_nuevo_grupo("Pruebas varias adicionales"); // Leer aclaración sobre estas pruebas en README.txt, sección Aclaraciones.
    DadaTablaDeCapacidadMinima_SiSeObligaColisionForzadaYSeBorraCasiTodo_NoSePuedeInsertarClaveRepetida();
    DadoUnDestructorNoNullYTablaConDatos_SiSeInsertanClavesRepetidas_LosDatosAntiguosSonCorrectamenteManejadosAlReemplazar();
    DadaTabla_SiSeRehasheaMasDeUnaVez_TodosLosElementosSeInsertanCorrectamente();

  return pa2m_mostrar_reporte();

}
