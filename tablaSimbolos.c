#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tablaSimbolos.h"

arbol tabla;

////FUNCIONES DE UN ARBOL////

void crea(arbol *A) { //Crea un arbol
    *A = NULL;
}

void destruye(arbol *A) { //Destruye un arbol
    if (*A != NULL) {
        destruye(&(*A)->izq);
        destruye(&(*A)->der);
        free(*A);
        *A = NULL;
    }
}

unsigned esVacio(arbol A) { //Comprueba si el arbol esta vacio
    return A == NULL;
}

void inserta(arbol* A, compLex* comp) { //Inserta un elemento en el arbol
    if (esVacio(*A)) {
        *A = (arbol) malloc(sizeof (struct nodo));
        (*A)->lexema = comp;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(comp->string, (*A)->lexema->string) < 0) {
        inserta(&(*A)->izq, comp);
    } else
        inserta(&(*A)->der, comp);
}

compLex* busca(arbol A, compLex *comp) { //Busca un elemento en el arbol
    if (esVacio(A))
        return NULL;
    else if (strcmp(comp->string, A->lexema->string) == 0)
        return A->lexema;
    else if (strcmp(comp->string, A->lexema->string) < 0)
        return busca(izq(A), comp);
    else
        busca(der(A), comp);
}

arbol izq(arbol A) { //Devuelve el nodo izq
    return A->izq;
}

arbol der(arbol A) { //Devuelve el nodo dcho
    return A->der;
}

void imprimeElemento(arbol* A) { //Imprime un elemento del arbol

    //Se recorre el arbol y se imprimen todos los elementos
    if (*A != NULL) {

        imprimeElemento(&(*A)->izq);

        if(!isFunc((*A)->lexema->string)){
          printf("<%s><%f>\n",(*A)->lexema->string, (*A)->lexema->num);
        }else{
          printf("<%s>\n", (*A)->lexema->string);
        }

        imprimeElemento(&(*A)->der);
    }

}


////FUNCIONES TABLA DE SIMBOLOS////

void crearTabla() { //Crea la tabla de simbolos
    crea(&tabla);
}

void destruyeTabla() { //Destruye la tabla de simbolos
    destruye(&tabla);
}

void inicializa() { //Inicializa la tabla de simbolos

    //Constantes
    insertaVarTabla("pi", CONSTANT, 3.14, NULL);
    insertaVarTabla("e", CONSTANT, 2.71, NULL);
    insertaVarTabla("L", CONSTANT, 6.023*pow(10, 23), NULL);

    //Funciones
    insertaVarTabla("sin", FUNC, 0, sin);
    insertaVarTabla("cos", FUNC, 0, cos);
    insertaVarTabla("atan", FUNC, 0, atan);
    insertaVarTabla("log", FUNC, 0, log);
    insertaVarTabla("exp", FUNC, 0, exp);
    insertaVarTabla("sqrt", FUNC, 0, sqrt);


}

int buscarEnTabla(compLex* comp) { //Busca un componente en la tabla de simbolos
    if (busca(tabla, comp) != NULL) {
        return 1;
    };
}

void insertarEnTabla(compLex* comp) { //Inserta un componente en la tabla de simbolos
    inserta(&tabla, comp);
}

void insertarPalReservada(char* lexema, int id) { //Inserta un componente léxico a partir de un lexema y un identificador

    //Definimos el componente lexico
    compLex* comp;
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(16);
    comp->id = id;

    //Pasamos el lexema a minusculas
    for (int i = 0; i < sizeof (lexema); i++) {
        comp->string[i] = tolower(lexema[i]);
    }

    //Insertamos el componente en la tabla
    insertarEnTabla(comp);
}

void imprimeTabla() { //Imprime el contenido de la tabla
    printf("\n/////TABLA DE SIMBOLOS/////\n");
    imprimeElemento(&tabla);
    printf("////////////////////////////\n\n");
}

void asignarID(compLex* comp) { //Busca un lexema en la tabla y le asigna el id correspondiente
    compLex* aux = busca(tabla, comp);
    comp->id = aux->id;
}

void insertaElemento(compLex* comp) { //Inserta un componente lexico en la tabla

    //Si el componente no esta en la tabla es un identificador
    if (buscarEnTabla(comp) != 1) {
        comp->id = 0; //Se le asigna como tipo ID
        insertarEnTabla(comp); //Se inserta en la tabla

        //Si esta en la tabla se le asigna el tipo que le corresponda
    } else {
        asignarID(comp);
    }
}

//Inserta un elemento en la tabla
void insertaVarTabla(char* name, int id, float num, void* func){

  //Si el elemento ya esta en la tabla
  if(getElem(name)!=NULL){

    if(getElem(name)->id == CONSTANT){
        printf("\tERROR -> No se pueden modificar constantes");
        return;
    }

    //Actualizamos el valor
    compLex* aux = getElem(name);
    aux->num = num;
    aux->func = func;

  //Si no esta lo insertamos
  }else{

    //Definimos el componente lexico
    compLex* comp;
    comp = (compLex *) malloc(sizeof (compLex));
    comp->string = (char *) malloc(strlen(name));
    comp->string = strdup(name);
    comp->id = id;
    comp->num = num;
    comp->func = func;

    //Insertamos el componente en la tabla
    insertarEnTabla(comp);
  }
}

//Devuelve un elemento por su nombre
compLex* getElem(char* name){
  compLex* comp;
  comp = (compLex *) malloc(sizeof (compLex));
  comp->string = strdup(name);
  return busca(tabla, comp);
}

int isFunc(char* name){
  if(getElem(name)-> id == FUNC){
    return 1;
  }
  return 0;
}
