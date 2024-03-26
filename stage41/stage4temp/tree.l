%{
#include "y.tab.h"
#include "tree.h"
#include<stdio.h>
#include<stdlib.h>
char *vari;
%}

%%

"begin" {return begin;}
"end" {return END;}
"write" {return WRITE;} 
"read" {return READ;}
"if" {return IF;}
"then" {return then;}
"else" {return Else;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"endwhile" {return endwhile;}
"continue" {return CONTINUE;}
"break" {return BREAK;}
"do" {return DO;}
"decl" {return DECL;}
"enddecl" {return ENDDECL;}
"int" {return INT;}
"str" {return STR;}

"<" {return LT;}
">" {return GT;}
"<=" {return LE;}
">=" {return GE;}
"==" {return EQ;}
"!=" {return NE;}

[0-9]+ {
    return NUM;
}
"="  {return ASSIGN;}
[a-z]+ {
    yylval.node=createTree(-1,tINT,yytext,tVAR,NULL,NULL,NULL);
    return ID;
}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return MUL;}  
"/" {return DIV;}
[ \t] {}
[' '] {}
[()] {return *yytext;}
[\n] {}
";" {return EOL;}
"," {return COMMA;}

%%

int yywrap(void)
{
    return 1;
}


