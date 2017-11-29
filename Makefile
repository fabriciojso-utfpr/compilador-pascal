all: monta compila testa clear
monta: lexico sintatico
lexico:
	bison sintatico.y -d
	flex -o lexico.c lexico.l
sintatico:
	bison sintatico.y
compila:
	gcc sintatico.tab.c -o pascal.bin
	gcc mepa.c -o mepa
testa:
	./pascal.bin ./pascal/exemplo.pas
clear:
	rm -f sintatico.tab.h sintatico.tab.c
	rm -f lexico.c
	rm -f pascal.bin
