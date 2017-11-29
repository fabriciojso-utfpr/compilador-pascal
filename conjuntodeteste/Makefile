all: constroi compila
constroi:
	bison sintatico.y -d
	flex -o lexico.c lexico.l
	bison sintatico.y

compila:
	gcc sintatico.tab.c -o pascal.bin
	gcc mepa.c -o mepa

testa:
	./pascal.bin entrada.pas saida.txt
clear:
	rm -f sintatico.tab.h sintatico.tab.c
	rm -f lexico.c 
	rm -f pascal.bin

prova:
	pascal.bin teste1.pas
	diff teste1.pas.mep teste1.pas.gabarito > resultado
	pascal.bin teste2.pas
	diff teste2.pas.mep teste2.pas.gabarito >> resultado
	pascal.bin teste3.pas
	diff teste3.pas.mep teste3.pas.gabarito >> resultado
	less resultado
limpa:
	rm -f *.mep
	rm -f resultado


