#define TAM_TSIMB 100 /*Tamanho da tabela de simbolos*/
#define TAM_PSEMA 100 /*Tamanho da pilha semantica*/
#include<stdarg.h>

int TOPO_TSIMB = 0;	/*Var. global do TOPO da tabela de simbolos */
int TOPO_PSEMA = 0; /*Var. global do TOPO da pilha semantica */
int NRO_POS_LOCAIS = 0; /*Numero de posicoes ocupadas por var. locais*/
int NIVEL_CORR = 0;	/*Nivel corrente*/
int ROTULO = 0; /*Proximo numero do rotulo*/
int CONTA_VARS = 0; /*conta o numero de variaveis declaradas*/
int PRE_READ = 0; /*Indica que esta sendo compilado um READ*/
int PRE_WRITE = 0; /*Indica que esta sendo compilado um WRITE*/
int POS_SIMB; /*Posicao de um simbolo na tabela de simbolos*/
char TIPO_VARIAVEL_ATRIBUICAO_PRIMEIRO;
char TIPO_VARIAVEL_ATRIBUICAO_SEGUNDO[2];
char num_str[6]; /*variaveis auxiliares para strings*/
char rot_str[8];
int aux; /*variavel tipo inteira auxiliar*/

FILE *arq_mep; /*arquivo com codigo MEPA gerado da compilacao*/

int numero_da_linha = 1; /*numero da linha do programa pascal*/
char atomo[30];

char nome_arq_pasc[100];

void msg_erro(char *msg, ...)
{
	char formato[255];
	va_list arg;
	va_start (arg, msg);
	sprintf(formato, "\n%s:%d: ", nome_arq_pasc, numero_da_linha);
	strcat(formato,msg);
	strcat(formato, "\n\n");
	printf("\nERRO no programa");
	vprintf(formato,arg);
	va_end (arg);
	fclose(arq_mep);
	exit(1);
}
typedef struct {
    char id[30];
    char tipo_primario;
    char tipo_secundario;
} tipo_temporario_struct;

typedef struct {
    tipo_temporario_struct primeiro;
    tipo_temporario_struct segundo;
} verificacao_de_tipos;

verificacao_de_tipos array_de_dados_para_verificacao;


struct elem_tab_simbolos
{
	char id[30];
	int nivel;
	int desloca;
	char tipo; /*-- TIPOS: i=integer,b=boolean,n=nulo; --*/
	char cat; /*-- CATEGORIAS: t=tipo, c=constante, p=procedimento, v=variavel --*/
} TSIMB [TAM_TSIMB], elem_tab;

int PSEMA[TAM_PSEMA];

int busca_simbolo (char *ident)
{
	int i = TOPO_TSIMB-1;
	for(;strcmp(TSIMB[i].id,ident) && i >= 0; i--);
	if(!strcmp(TSIMB[i].id,ident))
		return i;
	else
		return -1;
}

void insere_simbolo (struct elem_tab_simbolos *elem)
{
	if(TOPO_TSIMB == TAM_TSIMB)
	{
		msg_erro("OVERFLOW - tabela de simbolos");
	}
	else
	{
		POS_SIMB = busca_simbolo (elem->id);
		if((POS_SIMB != -1) && (TSIMB[POS_SIMB].nivel == NIVEL_CORR))
		{
			msg_erro ("Identificador [%s] ja declarado", elem->id);
		}
		TSIMB[TOPO_TSIMB] = *elem;
		TOPO_TSIMB++;
	}
}

void insere_variavel (char *ident)
{
	strcpy(elem_tab.id,ident);
	elem_tab.cat = 'v';
	elem_tab.nivel = NIVEL_CORR;
	elem_tab.desloca = NRO_POS_LOCAIS++;
	elem_tab.tipo = 'n';
	insere_simbolo (&elem_tab);
}

void atualiza_variaveis(char *tipo){
    char tipoFinal;
    int posicao;
    if(!strcmp(tipo, "boolean")){
        tipoFinal = 'b';
    }else{
        tipoFinal = 'i';
    }
    for(posicao=0;posicao<TOPO_TSIMB;posicao++){
        if(TSIMB[posicao].tipo == 'n'){
            TSIMB[posicao].tipo = tipoFinal;
        }
    }
}

void elimina_simbolos (int n)
{
	if((TOPO_TSIMB - n) < 0)
	{
		msg_erro ("UNDERFLOW - tabela de simbolos");
	}
	else
		TOPO_TSIMB -= n;
}

void inic_estruturas (void)
{
	/*insercao do simbolo INTEGER*/
	strcpy(elem_tab.id, "integer");
	elem_tab.cat = 't';
	elem_tab.nivel = -1;
	insere_simbolo (&elem_tab);

	/*insercao do simbolo boolean*/
	strcpy(elem_tab.id, "boolean");
	elem_tab.cat = 'b';
	elem_tab.nivel = -1;
	insere_simbolo (&elem_tab);

	/*insercao das constantes pre definidas FALSE e TRUE*/
	strcpy(elem_tab.id, "false");
	elem_tab.cat = 'c';
	elem_tab.nivel = -1;
    elem_tab.tipo = 'b';
    
	insere_simbolo (&elem_tab);
	strcpy(elem_tab.id, "true");
	elem_tab.cat = 'c';
	elem_tab.nivel = -1;
    elem_tab.tipo = 'b';
	insere_simbolo (&elem_tab);

	/*insercao dos procedimentos pre definidos READ e WRITE*/
	strcpy(elem_tab.id,"read");
	elem_tab.cat = 'p';
	elem_tab.nivel = -1;
	insere_simbolo (&elem_tab);
	strcpy(elem_tab.id,"write");
	elem_tab.cat = 'p';
	elem_tab.nivel = -1;
	insere_simbolo (&elem_tab);
}

void empilha (int n)
{
	if(TOPO_PSEMA == TAM_PSEMA)
	{
		msg_erro("OVERFLOW - Pilha semantica");
	}
	PSEMA[TOPO_PSEMA] = n;
	TOPO_PSEMA++;
}

int desempilha ()
{
	if(TOPO_PSEMA == 0)
	{
		msg_erro("UNDERFLOW - Pilha semantica");
	}
	return PSEMA[--TOPO_PSEMA];
}

void itoa (int nro,char *str)
{
	int i = 0, j = 0;
	char nro_str[6];
	while (nro)
	{
		nro_str[i++] = (nro % 10) + 49;
		nro = nro/10;
	}
	while (i) str[j++] = nro_str[--i];
	str [j] = '\0';
}
