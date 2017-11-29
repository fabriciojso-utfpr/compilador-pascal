#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

int mostra_carga = 0;
int mostra_pilha = 0;
int mostra_instrucao = 0;
int executa_getchar = 0;

FILE *arq;
struct {
	char inst[5];
	int rot, op1, op2, op3;
} P[500];
int L[50];
int M[500];
int D[50];
int s;
int i;
char rotulo[20], instrucao[20], operando[20];

int le_instrucao (char*,char*,char*);
void carrega_rotulo (char *);
void carrega_instrucao (char*,char*);

int main (int argc, char **argv)
{
	char numstr[5], nome_arq[80];
	int temp, temp1, k, fim, conta_col;
	--argc;
	++argv;

	if(argc == 0)
	{
		printf("\nMEPA - Maquina de Execucao PAscal:");
		printf("\nUso: MEPA <nome do programa>[.mep] [+cpi]");
		printf("\n\nc = mostrar carga do programa");
		printf("\np = mostrar pilha de execucao");
		printf("\ni = mostrar instrucao");
		printf("\ng = executar getchar() entre cada instrucao\n");
		exit(1);
	}
	strcpy (nome_arq, argv[0]);
	if (!strstr (nome_arq, ".mep"))
		strcat (nome_arq, ".mep");
	if ((arq = fopen(nome_arq, "r")) == NULL)
	{
		printf("\nErro de abertura");
		printf("\n[%s] nao existe\n\n", nome_arq);
		exit(1);
	}


	if(argc > 1)
	{
		if (strchr (argv[1], 'c'))
			mostra_carga = 1;
		if (strchr (argv[1], 'p'))
			mostra_pilha = 1;
		if (strchr (argv[1], 'i'))
			mostra_instrucao = 1;
		if (strchr (argv[1], 'g'))
			executa_getchar = 1;
		if (!strchr (argv[1], 'c') && !strchr (argv[1], 'P') && !strchr (argv[1], 'g') && !strchr (argv[1], 'i'))
		{
			printf("\nErro: [%s] nao e parametro valido\n\n", argv[1]);
			exit(1);
		}
	}

	i = 0;
	fim = 0;
	while(!fim)
	{
		fim = le_instrucao (rotulo, instrucao, operando);
		printf("++++++%s\n",rotulo);
		if(!fim)
		{
			carrega_rotulo(rotulo);
			carrega_instrucao(instrucao,operando);
			if(mostra_carga)
			{
				printf("\n#%3d: %5s %4d %4d %4d %4d", i, P[i].inst,P[i].rot,P[i].op1,P[i].op2, P[i].op3);
				if (!((i+1) % 20)) getchar();
			}
			i++;
		}
		else break;
	}
	fclose(arq);

	i = 0;
	while(1)
	{
		if(mostra_instrucao)
			printf("\nIntrucao #%3d : %s Operandos : p=%d m=%d n=%d k=%d ",i,P[i].inst, P[i].rot, P[i].op1, P[i].op2, P[i].op3);

		if(!strcmp(P[i].inst, "CRCT"))
		{
			s++;
			M[s] = P[i].op1;
			i++;
		} else
		if(!strcmp(P[i].inst, "CRVL"))
		{
			s++;
			M[s] = M[D[P[i].op1] + P[i].op2];
			i++;
		} else
		if(!strcmp(P[i].inst, "CREN"))
		{
			s++;
			M[s] = D[P[i].op1] + P[i].op2;
			i++;
		} else
		if(!strcmp(P[i].inst, "ARMZ"))
		{
			M[D[P[i].op1]+P[i].op2] = M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CRVI"))
		{
			s++;
			M[s] = M[M[D[P[i].op1]+P[i].op2]];
			i++;
		} else
		if(!strcmp(P[i].inst, "ARMI"))
		{
			M[M[D[P[i].op1]+P[i].op2]] = M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "SOMA"))
		{
			M[s-1] = M[s-1] + M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "SUBT"))
		{
			M[s-1] = M[s-1] - M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "MULT"))
		{
			M[s-1] = M[s-1] * M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "DIVI"))
		{
			M[s-1] = M[s-1] / M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "INVR"))
		{
			M[s] = -M[s];
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CONJ"))
		{
			if(M[s-1] && M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "DISJ"))
		{
			if(M[s-1] || M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "NEGA"))
		{
			M[s] = 1-M[s];
			i++;
		} else
		if(!strcmp(P[i].inst, "CMME"))
		{
			if(M[s-1] < M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CMMA"))
		{
			if(M[s-1] > M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CMIG"))
		{
			if(M[s-1] == M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CMDG"))
		{
			if(M[s-1] != M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CMEG"))
		{
			if(M[s-1] <= M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "CMAG"))
		{
			if(M[s-1] >= M[s])
				M[s-1] = 1;
			else
				M[s-1] = 0;
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "DSVS"))
		{
			i = L[P[i].rot];
		} else
		if(!strcmp(P[i].inst, "DSVF"))
		{
			if (M[s] == 0)
				i = L[P[i].rot];
			else
				i++;
			s--;
		} else
		if(!strcmp(P[i].inst, "NADA"))
		{
			i++;
		} else
		if(!strcmp(P[i].inst, "PARA"))
		{
			printf("\n\n");
			exit(1);
		} else
		if(!strcmp(P[i].inst, "LEIT"))
		{
			s++;
			printf("?");
			scanf("%s", numstr);
			M[s] = atoi(numstr);
			i++;

		} else
		if(!strcmp(P[i].inst, "IMPR"))
		{
			printf("\nSaida = %d", M[s]);
			s--;
			i++;
		} else
		if(!strcmp(P[i].inst, "AMEM"))
		{
			s = s + P[i].op1;
			i++;
		} else
		if(!strcmp(P[i].inst, "DMEM"))
		{
			s = s - P[i].op1;
			i++;
		} else
		if(!strcmp(P[i].inst, "INPP"))
		{
			s = -1;
			D[0] = 0;
			i++;
		} else
		if(!strcmp(P[i].inst, "ENRT"))
		{
			s = D[P[i].op1] + P[i].op2 - 1;
			i++;
		} else
		if(!strcmp(P[i].inst, "CREG"))
		{
			M[s+1] = L[P[i].rot];
			M[s+2] = D[P[i].op1];
			M[s+3] = P[i].op1;
			s = s + 3;
			i++;
		} else
		if(!strcmp(P[i].inst, "CHPR"))
		{
			M[s+1] = i + 1;
			M[s+2] = D[P[i].op1];
			M[s+3] = P[i].op1;
			s = s + 3;
			i = L[P[i].rot];
		} else
		if(!strcmp(P[i].inst, "CHPP"))
		{
			M[s+1] = i + 1;
			M[s+2] = D[P[i].op3];
			M[s+3] = P[i].op3;
			s = s + 3;
			temp = M[D[P[i].op1]+P[i].op2 + 2];
			D[temp] = M[D[P[i].op1]+P[i].op2 + 1];
			i = M[D[P[i].op1]+P[i].op2];
			while (temp >= 2)
			{
				D[temp - 1] = M[D[temp] - 1];
				temp--;
			}
			i = M[D[P[i].op1]+P[i].op2];
		} else
		if(!strcmp(P[i].inst, "ENPR"))
		{
			s++;
			M[s] = D[P[i].op1 - 1];
			D[P[i].op1] = s + 1;
			i++;
		} else
		if(!strcmp(P[i].inst, "RTPR"))
		{
			temp = M[s-1];
			D[temp] = M[s-2];
			temp1 = i;
			i = M[s-3];
			s = s - (P[temp1].op1 + 4);
			while (temp >= 2)
			{
				D[temp-1] = M[D[temp]-1];
				temp--;
			}
		}
		if (mostra_pilha)
		{
			printf("\nPilha Execucao:\n");
			conta_col = 1;
			for(k = 0; k <= s; k++)
			{
				printf("%4d", M[k]);
				if(conta_col % 20 == 0)
					printf("\n");
				conta_col++;
			}
			printf("\nRegs. de Base..: ");
			for(k = 0; k <= 10; k++)
				printf("%4d", D[k]);
			printf("\n\n");
		}
		if (executa_getchar)
			getchar();
	}
}

int le_instrucao(char *r, char *i, char *o)
{
	char branco[10], *aux;
	fgets(r,9,arq);
	fgets(branco,6,arq);
	fgets(i,6,arq);
	aux = strchr (i,'\n');
	if (!aux)
	{
		aux = strchr(i,' ');
		*aux = '\0';
		fgets (branco, 7, arq);
		fgets (o,9,arq);
		if(!strcmp(o,""))
			return 1;
		aux = strchr(o,'\n');
		*aux = '\0';
	}
	else
	{
		strcpy(o,"");
		*aux = '\0';
	}
	return 0;
}

void carrega_rotulo (char *r)
{
	char rot[5], *aux;
	int j;
	aux = strchr (r, 'L');
	if(aux)
	{
		j = 0;
		aux++;
		while(*aux != ':')
			rot[j++] = *aux++;
		rot[j] = '\0';
		j = atoi (rot);
		L[j] = i;
	}
	printf("**********%s  L[%d]=%d\n",rot,j,i);
}


void carrega_instrucao(char *inst, char *oper)
{
	char num[5], *aux;
	int j;
	strcpy(P[i].inst, inst);
	P[i].rot = P[i].op1 = P[i].op2 = P[i].op3 = 0;
	if(*oper == '\0') return;
	if(strchr (oper, 'L'))
	{
		j = 0;
		oper++;
		while(*oper != ',' && *oper != '\0')
			num[j++] = *oper++;
		num[j] = '\0';
		P[i].rot = atoi (num);
		if (*oper == '\0') return;
		oper++;
	}
	j = 0;
	while(*oper != ',' && *oper != '\0' && *oper != ' ')
		num[j++] = *oper++;
	num[j] = '\0';
	P[i].op1 = atoi(num);
	if(*oper == '\0' || *oper == ' ') return;
	oper++;
	j = 0;
	while(*oper != ',' && *oper != '\0' && *oper != ' ')
		num[j++] = *oper++;
	num[j] = '\0';
	P[i].op2 = atoi(num);
	if(*oper == '\0' || *oper == ' ') return;
	oper++;
	j = 0;
	while(*oper != ',' && *oper != '\0' && *oper != ' ')
		num[j++] = *oper++;
	num[j] = '\0';
	P[i].op3 = atoi(num);
	if(*oper == '\0' || *oper == ' ') return;
}
