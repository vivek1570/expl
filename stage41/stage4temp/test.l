%{
    #include<stdio.h>
    #include "y.tab.h"
%}


%%

"decl" {return DECL;}
"enddecl" {return ENDDECL;}
"int" {return INT;}
"str" {return STR;}

[a-z0-9]+ {
    return  ID;}

[\n] {}
[' '] {}

";" {return EOL;}

"," {return COMMA;}

%%

int yywrap(void)
{
    return 1;
}