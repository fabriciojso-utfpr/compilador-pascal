%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "utils.c"
#include "lexico.c"

void yyerror (char *s);
%}
%expect 1
%start programa
%token S_DIFERENTE
%token S_DOISPONTOS
%token S_FECHAPAR
%token S_MAIGUAL
%token S_MEIGUAL
%token S_PROGRAM
%token S_PONTOVG
%token S_IDENTIF
%token S_ABREPAR
%token S_VIRGULA
%token S_ATRIBUI
%token S_NUMERO
%token S_WHILE
%token S_WRITE
%token S_READ
%token S_MAIOR
%token S_MENOR
%token S_IGUAL
%token S_MENOS
%token S_VEZES
%token S_PONTO
%token S_BEGIN
%token S_THEN
%token S_ELSE
%token S_MAIS
%token S_VAR
%token S_END
%token S_DIV
%token S_AND
%token S_NOT
%token S_IF
%token S_DO
%token S_OR

%%

programa
	: S_PROGRAM S_IDENTIF S_PONTOVG
		{fprintf(arq_mep, "INPP\n");}
	  bloco S_PONTO
		{fprintf(arq_mep, "PARA\n");}
		{fprintf(arq_mep, "FIM\n");}
	;
bloco
	: variaveis corpo
	;
variaveis
	: /*vazio*/
	| S_VAR
		{CONTA_VARS = 0;}
	  declaracoes_de_vars
		{fprintf (arq_mep,"AMEM\t\t%d\n",CONTA_VARS);}
	;
declaracoes_de_vars
	: declaracoes_de_vars
lista_de_identificadores S_DOISPONTOS S_IDENTIF {atualiza_variaveis(atomo);} S_PONTOVG
	| lista_de_identificadores S_DOISPONTOS S_IDENTIF {atualiza_variaveis(atomo);} S_PONTOVG
	;
lista_de_identificadores
	: lista_de_identificadores S_VIRGULA S_IDENTIF
		{insere_variavel (atomo); CONTA_VARS++;}
	| S_IDENTIF
		{insere_variavel (atomo); CONTA_VARS++;}
	;
corpo
	: S_BEGIN sequencia_comandos S_END
	;
sequencia_comandos
	: sequencia_comandos S_PONTOVG comando_sem_rotulo
	| comando_sem_rotulo
	;
comando_sem_rotulo
	: /*vazio*/
	| chamada_comando_escrita
	| chamada_comando_leitura
	| atribuicao
	| composto
	| condicional
	| repetitivo
	;
chamada_comando_escrita
	: S_WRITE
	  S_ABREPAR lista_de_expressoes S_FECHAPAR
	;
chamada_comando_leitura
	: S_READ
	  S_ABREPAR lista_de_variaveis S_FECHAPAR
	;
lista_de_expressoes
	: /*vazio*/
	| lista_de_expressoes S_VIRGULA
	  expressao
		{fprintf (arq_mep, "IMPR\n");}
	| expressao
		{fprintf (arq_mep, "IMPR\n");}
	;
lista_de_variaveis
	: /*vazio*/
	| lista_de_variaveis S_VIRGULA
		{fprintf (arq_mep, "LEIT\n");}
	  S_IDENTIF
		{
			POS_SIMB = busca_simbolo (atomo);
			if (POS_SIMB == -1)
				msg_erro ("Variavel [%s] nao declarada!", atomo);
			else {
				fprintf (arq_mep,"%17s%7s%d,%d\n",
					"ARMZ", "", 0, TSIMB[POS_SIMB].desloca);
			}
		}
	| {fprintf (arq_mep, "%17s\n", "LEIT");}
	  S_IDENTIF
		{
			POS_SIMB = busca_simbolo (atomo);
			if (POS_SIMB == -1)
				msg_erro ("Variavel [%s] nao declarada!", atomo);
			else {
				fprintf (arq_mep,"%17s%7s%d,%d\n",
					"ARMZ", "", 0, TSIMB[POS_SIMB].desloca);
			}
		}
	;
atribuicao
	: S_IDENTIF
		{
			POS_SIMB = busca_simbolo (atomo);
			if (POS_SIMB == -1)
				msg_erro ("Variavel [%s] nao declarada!", atomo);
			else {
                empilha (TSIMB[POS_SIMB].desloca);
                strcpy(array_de_dados_para_verificacao.primeiro.id, TSIMB[POS_SIMB].id);
                array_de_dados_para_verificacao.primeiro.tipo_primario = TSIMB[POS_SIMB].tipo;
			}
		}
	  S_ATRIBUI expressao
		{
            if(
                array_de_dados_para_verificacao.primeiro.tipo_primario ==
                array_de_dados_para_verificacao.segundo.tipo_primario
                ||
                array_de_dados_para_verificacao.primeiro.tipo_primario ==
                array_de_dados_para_verificacao.segundo.tipo_secundario
                ){
                    
            fprintf(arq_mep,"%17s%7s%d,%d\n", "ARMZ", "", 0, desempilha());
                }else{
                    msg_erro ("%s é %s e %s é %s", array_de_dados_para_verificacao.primeiro.id,
                    (array_de_dados_para_verificacao.primeiro.tipo_primario == 'b' ? "booleano" : "inteiro"),
                                                  
                                                  array_de_dados_para_verificacao.segundo.id,
                                                  (array_de_dados_para_verificacao.segundo.tipo_primario == 'b' ? "booleano" : "inteiro")
                                                  );
                }
        }
	;
composto
	: S_BEGIN lista_comandos_sem_rotulo S_PONTOVG S_END
	;
lista_comandos_sem_rotulo
	: lista_comandos_sem_rotulo S_PONTOVG comando_sem_rotulo
	| comando_sem_rotulo
	;
condicional
	: S_IF
	  expressao
		{
			itoa (++ROTULO, num_str);
			strcpy (rot_str, "L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%17s%7s%s\n", "DSVF", "", rot_str);
			empilha (ROTULO);
		}
	  S_THEN
	  comando_sem_rotulo
	  alternativa_cond
		{
			itoa (desempilha(), num_str);

			strcpy (rot_str, "L");

			strcat (rot_str, num_str);

			fprintf (arq_mep,"%7s:%9s\n", rot_str, "NADA");

		}
	;
alternativa_cond
	: /*vazio*/
	| S_ELSE
		{
			aux = desempilha();
			itoa (++ROTULO, num_str);
			strcpy (rot_str,"L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%17s%7s%s\n", "DSVS", "", rot_str);
			empilha (ROTULO);
			itoa (aux, num_str);
			strcpy(rot_str,"L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%7s:%9s\n", rot_str, "NADA");
		}
	  comando_sem_rotulo
	;
repetitivo
	: S_WHILE
		{
			itoa (++ROTULO,num_str);
			strcpy (rot_str, "L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%7s:%9s\n", rot_str, "NADA");
			empilha (ROTULO);
		}
	  expressao
		{
			itoa (++ROTULO,num_str);
			strcpy (rot_str, "L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%17s%7s%s\n", "DSVF", "", rot_str);
			empilha (ROTULO);
		}
	  S_DO comando_sem_rotulo
		{
			aux = desempilha();
			itoa (desempilha(), num_str);
			strcpy (rot_str, "L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%17s%7s%s\n", "DSVS", "", rot_str);
			itoa (aux, num_str);
			strcpy (rot_str, "L");
			strcat (rot_str, num_str);
			fprintf (arq_mep,"%7s:%9s\n", rot_str, "NADA");
		}
	;
expressao
	: expressao_simples S_IGUAL expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMIG"); }
	| expressao_simples S_DIFERENTE expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMDG"); }
	| expressao_simples S_MENOR expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMME"); }
	| expressao_simples S_MAIOR expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMMA"); }
	| expressao_simples S_MEIGUAL expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMEG"); }
	| expressao_simples S_MAIGUAL expressao_simples
		{ fprintf (arq_mep, "%17s\n", "CMAG"); }
	| expressao_simples
	;
expressao_simples
	: expressao_simples S_MAIS termo
		{ fprintf (arq_mep, "%17s\n", "SOMA"); }
	| expressao_simples S_MENOS termo
		{ fprintf (arq_mep, "%17s\n", "SUBT"); }
	| expressao_simples S_OR termo
		{ fprintf (arq_mep, "%17s\n", "DISJ"); }
	| S_MAIS termo
	| S_MENOS termo
		{ fprintf (arq_mep, "%17s\n", "INVR"); }
	| termo
	;
termo
	: termo S_VEZES fator
		{ fprintf (arq_mep, "%17s\n", "MULT"); }
	| termo S_DIV fator
		{ fprintf (arq_mep, "%17s\n", "DIVI"); }
	| termo S_AND fator
		{ fprintf (arq_mep, "%17s\n", "CONJ"); }
	| fator
	;
fator
	: S_IDENTIF
		{
			POS_SIMB = busca_simbolo (atomo);
			if (POS_SIMB == -1)
				msg_erro ("Variavel [%s] nao declarada!", atomo);
			else
                strcpy(array_de_dados_para_verificacao.segundo.id, TSIMB[POS_SIMB].id);
                array_de_dados_para_verificacao.segundo.tipo_primario = TSIMB[POS_SIMB].tipo;
                array_de_dados_para_verificacao.segundo.tipo_secundario = TSIMB[POS_SIMB].tipo;
                
				fprintf (arq_mep,"%17s%7s%d,%d\n",
					"CRVL", "", 0, TSIMB[POS_SIMB].desloca);
		}
	| S_NUMERO
		{ fprintf (arq_mep,"%17s%7s%-5s\n", "CRCT", "", atomo);
            int aux_temp = atoi(atomo);
            
            strcpy(array_de_dados_para_verificacao.segundo.id, atomo);
            array_de_dados_para_verificacao.segundo.tipo_primario = 'i';
          
            if(aux_temp == 0 || aux_temp == 1){
                array_de_dados_para_verificacao.segundo.tipo_secundario = 'b';
            }else{
                 array_de_dados_para_verificacao.segundo.tipo_secundario = 'i';
            }
        }
	| S_ABREPAR expressao S_FECHAPAR
	| S_NOT fator
		{ fprintf (arq_mep,"%17s\n", "NEGA"); }
	;
%%



void yyerror (char *s)
{
	printf ("\n%s:%d: ERRO SINTATICO \n\n", nome_arq_pasc, numero_da_linha);
}

void separa_nome_arquivo (char *si, char *so)
{
	int i,j;
	for (i = strlen (si); si[i] != '/' && i >= 0; i--);
	for (i++, j = 0; si[i]; i++,j++)
		so[j] = si[i];
	so[j] = '\0';
}

int main (int argc, char **argv)
{
	char msg[40];
	char nome[100];
	inic_estruturas ();

	if (argc == 2) {
		strcpy(nome,argv[1]);
		strcat (nome,".mep");
		if ((arq_mep = fopen (nome, "w")) == NULL) {
			sprintf (msg, "Impossivel criar o arquivo [%s]!", nome);
			msg_erro (msg);
		}

		if ((yyin = fopen (argv[1], "r")) == NULL) {
			sprintf (msg, "Arquivo [%s] nao encontrado!", argv[1]);
			msg_erro (msg);
		}
		printf("ok.\n");

		if (!yyparse ()) {
			printf ("\nPrograma CORRETO!");
			printf ("\nNumero de linhas compiladas: %d\n\n\n", numero_da_linha);
		}
		fclose (arq_mep);
	}
	else {
		printf ("\nCompilador PASCAL");
		printf ("\nUso: %s <nome_do_programa>[.pas]\n\n\n", argv[0]);
	}
}

