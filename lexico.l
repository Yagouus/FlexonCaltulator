%{
//Librerias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.h"
#include "anaSintactico.tab.h"

//Funciones
void leerArchivo();

%}

%option noyywrap

/*-----DEFINICIONES-----*/
ENTERO              [[:digit:]_]+
FLOAT               [[:digit:]]*\.[[:digit:]]+
EXP                 [eE][-+]?[[:digit:]_]+
ID                  [[:alnum:]_]*
IGNORAR             [[:blank:]]

%x load

%%


"exit"          {yyterminate();}
"tabla;"        {imprimeTabla();}

"in:"           BEGIN(load);
<load>{ID}      leerArchivo(yytext);
<load>";"       BEGIN(INITIAL);

{IGNORAR}       {}

{FLOAT}{EXP}    { yylval.num=atof(yytext); return NUM; }

{ENTERO}{EXP}   { yylval.num=atof(yytext); return NUM;}

{FLOAT}         {yylval.num=atof(yytext); return NUM;}

{ENTERO}        {yylval.num=atof(yytext); return NUM;}

{ID}            {  yylval.id = (char *) malloc(sizeof (yytext));
                   yylval.id = strdup(yytext);
                   return VAR;
                 }

";"		          {return PTC;}
"="             {return EQL;}
"("             {return PRI;}
")"             {return PRD;}
"+"             {return MAS;}
"-"             {return MNS;}
"*"             {return TMS;}
"/"             {return DIV;}
"^"             {return POW;}
.               {printf("ERROR-> Caracter no reconocido");};

<<EOF>>		      {yyterminate();}

%%

void leerArchivo(char* fichero){
    if(fichero == NULL){
        yyin = fopen("regression.d", "r");
    }else{
        yyin = fopen(fichero, "r");
    }

    if(yyin == 0){
        //error(FICHERO);
    }
}
