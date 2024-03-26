%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    extern char * yytext;
    int yyerror();  
    int yylex();
    extern FILE *yyin;
    extern int  yyleng;
    char type[20];
%}


%token DECL ENDDECL INT STR ID EOL COMMA

%type Declarations DeclList Decl Type VarList

%%

Declarations : DECL DeclList ENDDECL EOL {
    printf("\nWe have Done it!!! \n");}|
     DECL ENDDECL EOL
    {
        printf("\nWe have Done it!!! \n");
    };

DeclList : DeclList Decl | Decl

Decl : Type VarList EOL {
    printf("in declare list\n");
}

Type : INT {
    strcpy(type,yytext);
}|STR {
   strcpy(type,yytext);
};

VarList : VarList COMMA ID {
    printf("ID=%s ",yytext);
    printf("type=%s ",type);
    printf("Length=%d\n\n",yyleng);
}| ID{
    printf("ID=%s ",yytext);
    printf("type=%s ",type);
    printf("Length=%d\n\n",yyleng);
};

%%

int yyerror()
{
    printf("have some error encountered\n");
    exit(0);
}

int main()
{
    yyin=fopen("input_l","r");
    yyparse();
    return 0;
}