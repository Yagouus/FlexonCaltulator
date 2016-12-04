calculadora:	
	bison -d anaSintactico.y
	flex lexico.l	
	gcc -o calculadora lex.yy.c anaSintactico.tab.c tablaSimbolos.c -lfl -lm


