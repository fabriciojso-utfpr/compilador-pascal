%option noyywrap
%{
#include <stdio.h>
#include "sintatico.tab.h"
%}

DIGIT	[0-9]
ID		[a-z][a-z0-9]*
%x coment_1
%x coment_2

%%
if		{return (S_IF);}
then		{return (S_THEN);}
else		{return (S_ELSE);}
while		{return (S_WHILE);}
do		{return (S_DO);}
program		{return (S_PROGRAM);}
var		{return (S_VAR);}
begin		{return (S_BEGIN);}
end		{return (S_END);}
div		{return (S_DIV);}
and		{return (S_AND);}
or		{return (S_OR);}
not		{return (S_NOT);}
write		{return (S_WRITE);}
read		{return (S_READ);}
"+"		{return (S_MAIS);}
"-"		{return (S_MENOS);}
"*"		{return	(S_VEZES);}
":="		{return	(S_ATRIBUI);}
":"		{return (S_DOISPONTOS);}
"."		{return	(S_PONTO);}
";"		{return (S_PONTOVG);}
","		{return (S_VIRGULA);}
"("		{return (S_ABREPAR);}
")"		{return (S_FECHAPAR);}
">"		{return (S_MAIOR);}
"<"		{return (S_MENOR);}
">="		{return (S_MAIGUAL);}
"<="		{return (S_MEIGUAL);}
"="		{return	(S_IGUAL);}
"<>"		{return	(S_DIFERENTE);}

{DIGIT}+	{strcpy(atomo, yytext);
			 return (S_NUMERO);}
{ID}		{strcpy(atomo, yytext);
			 return (S_IDENTIF);}

[ \t]	/*salta espaço em branco*/
[\n]	{numero_da_linha++;}

"(*"					BEGIN (coment_1);
<coment_1>[^*\n]*		/*salta caracteres diferentes de '*' e \n */
<coment_1>[*]+[^*)\n]	/*salta car's '*' nao seguidos por *) e \n */
<coment_1>\n			{numero_da_linha++;}
<coment_1>"*"+")"		BEGIN (INITIAL);

"{"						BEGIN (coment_2);
<coment_2>[^}\n]*		/*salta car's diferentes de '}' e \n*/
<coment_2>\n			{numero_da_linha++;}
<coment_2>"}"			BEGIN (INITIAL);


<<EOF>> 	{return (0);}
.			{msg_erro ("Simbolo Desconhecido [%s]", yytext);}


