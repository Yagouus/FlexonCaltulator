%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lex.yy.h"

extern int yylex();
void yyerror(char* s);
void ayuda();
%}

%union {
float	num;
char	*id;
}

//Expresion inicial
%start input

%token <num> NUM
%token <id> VAR

%token PTC
%token PRI
%token PRD
%right EQL
%left MAS
%left MNS
%left TMS
%left DIV
%left PRI
%left NEG
%right POW


%type <num> EXP
%type <num> SET
%type <num> OP


%%

input:
        | input line
;

line:    EXP PTC { printf ("\t>%.4f\n",$1); }

;

EXP:    NUM   {$$ = $1;}
        | VAR   {if(getElem($1)){ $$ = getElem($1)->num; } else { yyerror(INITVAR); } }
        | SET
        | VAR PRI EXP PRD {if(getElem($1)){ if(isFunc($1)){ $$ = getElem($1)->func($3);}} else { yyerror(NOTFUNC); } }
        | OP
        | PRI EXP PRD {$$ = $2;}
;

OP:     EXP MAS EXP   {$$ = $1 + $3;}
        | EXP MNS EXP {$$ = $1 - $3;}
        | EXP TMS EXP {$$ = $1 * $3;}
        | EXP DIV EXP {if($3 != 0){$$ = $1 / $3;} else { yyerror(DIVCERO); } }
        | MNS EXP %prec NEG {$$ = -$2;}
        | EXP POW EXP {$$ = pow($1, $3);}
;

SET:    VAR EQL EXP   {insertaVarTabla($1, VARIABLE, $3, NULL); $$ = $3;}
;


%%

void yyerror(char* s){
  printf ("\tERROR -> %s\n", s);
}

int main(int argc,char **argv)
{
  //Menu de ayuda
  if(argc > 1){
      if(!strcmp(argv[1], "--help")){
        ayuda();
      }else{
        leerArchivo(argv[1]);
      }
    }

  //Creamos, inicializamos y mostramos la TS
  crearTabla();
  inicializa();
  imprimeTabla();

  //Compilamos
  yyparse();

  //Volvemos a imprimir la TS
  imprimeTabla();
  destruyeTabla();
  return 0;
}

void ayuda(){
  printf("AYUDA:");

  printf("\n--IMPORTANTE");
  printf("\n\t-Para que una instrucción se ejecute ha de terminar en \";\"");

  printf("\n--Constantes");
  printf("\n\t-Pi");
  printf("\n\t-E");
  printf("\n\t-L (Numero de Avogadro)");

  printf("\n--Funciones");
  printf("\n\t-Sin (seno)");
  printf("\n\t-Cos (coseno)");
  printf("\n\t-Atan (arco tangente)");
  printf("\n\t-Log (logaritmo)");
  printf("\n\t-Exp (exponencial");
  printf("\n\t-Sqrt (raiz cuadrada)");

  printf("\n--Cargar Archivo");
  printf("\n\t-Escribir el comando \"in:nombrefichero\"");
  printf("\n\t-O ejercutar el programa de la forma \"calculadora fichero\"");

  printf("\n--Tabla de simbolos");
  printf("\n\t-Escrbir el comando \"tabla\" para ver el contenido de la tabla de simbolos");

  printf("\n--Terminar el programa");
  printf("\n\t-Escribir el comando \"exit\"");

  printf("\n");
  exit(0);
}
