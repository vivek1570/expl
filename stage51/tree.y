%{
    #include<stdio.h>
    #include<stdlib.h>
    #include "tree.h"
    #include "tree.c"
    #include "symbol.h"
    #include "symbol.c"
    #include<string.h>
    FILE *targetFile;
    extern FILE *yyin;
    int yylex(void);
    extern char* yytext;
    int type;
    int yyerror(char const *s);
    struct paramstruct*paramlist=NULL;
    int flabel=0;
    struct typi* typ=NULL;
    struct paramstruct*paramtemp=NULL;
    %}

    %union{
        struct tnode *node;
    }

    %token<node> NUM PLUS MINUS MUL DIV  WRITE READ EOL ASSIGN ID IF then Else Return
    %token<node>  LE GE LT GT NE EQ MSG MOD 
    %token<node> DECL ENDDECL INT STR COMMA MAIN START END ENDIF endwhile WHILE DO BREAK CONTINUE

    %left EQ LE GE LT GT NE
    %left PLUS MINUS
    %left MUL DIV
    %left MOD
    %right ASSIGN

    
    %type <node> expr Program Stmt Slist InputStmt OutputStmt AsgStmt Ifstmt Whilestmt BreakStmt ContStmt VAR 

    %type <node> FdefBlock Fdef MainBlock ArgList Body GdeclBlock GdeclList GidList Gid Type 

    %type <node> ParamList LdeclBlock  LDecList LDecl ReturnStmt


    
   
    
    %%
    //Main building blocks-------------------------

    Program:GdeclBlock FdefBlock MainBlock 
                        {
                            printf("Done!!\n");
                            initial(targetFile);
                            printSTable();
                        };
            | GdeclBlock MainBlock 
                        {
                            printf("Done!!\n");
                            initial(targetFile);
                            printSTable();
                        };
            |MainBlock   
                        {  
                            printf("done!!\n");
                            initial(targetFile);
                        };

    //Global declaration----------------------------

    GdeclBlock: DECL GdeclList ENDDECL{};
                | DECL ENDDECL{};

    GdeclList:GdeclList Gdecl{};
            | Gdecl{};

    Gdecl:Type GidList EOL{};

    GidList:GidList COMMA Gid  {};
            | Gid{};

    Gid: ID
                        {
                            if(Lookup($1->varname)==NULL)
                            Install($1->varname,type,1,NULL,-1);
                            else{
                                printf("redclare error of %s\n",$1->varname);
                                exit(1);
                            }
                        }
        | ID '['NUM']'
                        {
                            if(Lookup($1->varname)==NULL)
                            Install($1->varname,type,$3->val,NULL,-1);
                            else{
                                printf("redclare error of the array %s\n",$1->varname);
                                exit(1);
                            }
                        }
        |ID '('ParamList')'
                        {
                            if(Lookup($1->varname)==NULL)
                            Install($1->varname,type,-1,paramlist,flabel);
                            else{
                                printf("The function redeclaration error %s\n",$1->varname);
                                paramlist=NULL;
                                exit(1);
                            }
                            flabel=flabel+1;
                            paramtemp=paramlist;
                            paramlist=NULL;
                            /* struct paramstruct *curr=paramlist;
                            while(curr!=NULL)
                            {
                                struct paramstruct *temp=curr;
                                curr=curr->next;
                                free(temp->name);
                                free(temp);
                            } */
                        };
    // _function_building blocks----------------------------

    FdefBlock:FdefBlock Fdef
                        {
                            $$=createTree(-1,-1,NULL,tFunconnect,$1,$2,NULL);
                        }
            |Fdef
                        {
                            $$=createTree(-1,-1,NULL,tFunconnect,$1,NULL,NULL);
                        }
                        ;

    Fdef: Type ID'('ParamList ')' '{' LdeclBlock Body'}'
                        {
                            int c=checkparam(paramlist,$2->varname);
                            if(c==-1)
                            {
                                paramlist=NULL;
                                printf("error has occured \n");
                                exit(1);
                            }


                            paramlist=NULL;
                            reset_local();
                            $$=$8;
                            printf("The ast of %s -----------\n",$2->varname);
                            prefix($$);
                            printf("\n\n");
                        };

    ParamList : ParamList COMMA Param {};| Param{};
                |  {} ;

    Param : Type ID
                        {
                                struct paramstruct *temp=(struct paramstruct*)malloc(sizeof(struct paramstruct));
                                temp->name=strdup($2->varname);
                                temp->type=type;
                                temp->next=paramlist;
                                paramlist=temp;                     
                        };

    MainBlock:INT MAIN '('')' '{' LdeclBlock Body '}'
                        {
                            $$=$7;
                            printf("The ast of Main -----------\n");
                            prefix($$);
                            printf("\n\n");
                            //here $1 represent the  root of the tree 
                            //1.free memory of Gtable and Local table and AST
                            reset_local();
                        };

    Type : INT 
                        {
                        type=tINT;
                        };
        | STR           { type=tSTR;};


    LdeclBlock : DECL LDecList ENDDECL{} 
                | DECL ENDDECL{};

    LDecList :LDecList LDecl{}; 
                | LDecl{};

    LDecl : Type IdList EOL {};

    IdList :IdList COMMA ID 
                        {
                            if(LLookup($3->varname)==NULL){LInstall($3->varname,type);}
                            else {printf("Redeclaraton error in %s\n",$3->varname);exit(1);}
                        }
            | ID        
                        {
                            if(LLookup($1->varname)==NULL)LInstall($1->varname,type);
                            else {printf("Redeclaraton error in %s\n",$1->varname);exit(1);}
                        };
    Body:START Slist END 
                        {
                            $$=$2;
                        };
    |START END{};

    Slist:Slist Stmt 
                        {
                            $$=createTree(-1,-1,NULL,tCONNECT,$1,$2,NULL);
                        };
    | Stmt
                        {
                            $$=$1;
                        };

    Stmt: InputStmt {$$=$1;}|
          OutputStmt{$$=$1;}|
          AsgStmt{ $$=$1;}|
          Ifstmt{$$=$1;}|
          Whilestmt{ $$=$1;}|
          BreakStmt { $$=$1;}|
          ContStmt {$$=$1;}|
          ReturnStmt{$$=$1;};
    
    ReturnStmt:Return expr EOL {$$=createTree(-1,-1,NULL,tReturn,$2,NULL,NULL);}
                | Return EOL {$$=createTree(-1,-1,NULL,tReturn,NULL,NULL,NULL);};
          
    BreakStmt: BREAK EOL {$$=createTree(-1,-1,NULL,tBREAK,NULL,NULL,NULL);};

    ContStmt: CONTINUE EOL {$$=createTree(-1,-1,NULL,tCONTINUE,NULL,NULL,NULL);};

    InputStmt:  READ '(' expr ')' EOL {$$=createTree(-1,-1,NULL,tREAD,$3,NULL,NULL);};

    OutputStmt: WRITE '(' expr ')' EOL {$$=createTree(-1,-1,NULL,tWRITE,$3,NULL,NULL);};
    
    AsgStmt: expr ASSIGN expr EOL { $$=createTree(-1,-1,NULL,tASSIGN,$1,$3,NULL);};

    Ifstmt: IF '(' expr ')' then Slist Else Slist ENDIF EOL {$$=createTree(-1,-1,NULL,tIF,$3,$6,$8);}
        | IF '(' expr ')' then Slist ENDIF EOL {$$=createTree(-1,-1,NULL,tIF,$3,$6,NULL);};

    Whilestmt: WHILE '(' expr ')' DO Slist endwhile EOL {$$=createTree(-1,-1,NULL,tWHILE,$3,$6,NULL);};

    expr:expr PLUS expr {$$=createTree(-1,-1,"+",tADD,$1,$3,NULL);}|
         expr MINUS expr {$$=createTree(-1,-1,"-",tSUB,$1,$3,NULL);}|
         expr MUL expr {$$=createTree(-1,-1,"*",tMUL,$1,$3,NULL);}|
         expr DIV expr{$$=createTree(-1,-1,"/",tDIV,$1,$3,NULL);}|
         expr LE expr {$$=createTree(-1,-1,NULL,tLE,$1,$3,NULL);}|
         expr GE expr {$$=createTree(-1,-1,NULL,tGE,$1,$3,NULL);}|
         expr LT expr {$$=createTree(-1,-1,NULL,tLT,$1,$3,NULL);}|
         expr GT expr {$$=createTree(-1,-1,NULL,tGT,$1,$3,NULL);}|
         expr NE expr {$$=createTree(-1,-1,NULL,tNE,$1,$3,NULL);}|
         expr EQ expr {$$=createTree(-1,-1,NULL,tEQ,$1,$3,NULL);}|
         expr MOD expr {$$=createTree(-1,-1,NULL,tMOD,$1,$3,NULL);}|
         NUM {$$=$1;}|
         MSG {$$=$1;}|
         VAR{$$=$1;}|
         ID '(' ')'
                        {
                            $1->Gentry=Lookup($1->varname);
                            if($1->Gentry==NULL)
                            {
                                printf("Function '%s' not declared!",$1->varname);
                                exit(1);
                            }
                            struct typi*temp=typ;
                            struct paramstruct*curr=paramtemp;
                            while(curr!=NULL)
                            {   
                                if(temp==NULL)
                                {
                                    printf("error,full arguments are not passed in Main program for %s\n",$1->varname);
                                    exit(1);
                                }
                               if(curr->type!=temp->type) 
                               {
                                printf("argument type error in '%s' in Main program\n",$1->varname);
                                exit(1);
                               }
                               
                               curr=curr->next;
                               temp=temp->next;
                            }


                            $1->type=$1->Gentry->type;
                            $$=createTree(-1,type,$1->varname,tFunction,NULL,NULL,NULL);
                        };
         |
         ID '(' ArgList ')'
                        {
                            $1->Gentry=Lookup($1->varname);
                            if($1->Gentry==NULL)
                            {
                                printf("Function '%s' not declared!",$1->varname);
                                exit(1);
                            }
                            //type check in argument list of a funtion and the arguments passed to a function also
                            struct typi*temp=typ;
                            struct paramstruct*curr=paramtemp;
                            
                            //type error check through the number of arguments
                            int y=0,x=0;
                            while(temp!=NULL){temp=temp->next;x++;};
                            while(curr!=NULL){curr=curr->next;y++;};
                            if(x!=y){printf("The number of argumnets are differ in '%s' function\n",$1->varname);exit(1);}
                            temp=typ;curr=paramtemp;

                            while(curr!=NULL)
                            {   
                                if(temp==NULL)
                                {
                                    printf("error,full arguments are not passed in Main program for %s\n",$1->varname);
                                    exit(1);
                                }
                               if(curr->type!=temp->type) 
                               {
                                printf("argument type error in '%s' in Main program\n",$1->varname);
                                exit(1);
                               }
                               
                               curr=curr->next;
                               temp=temp->next;
                            }


                            $1->type=$1->Gentry->type;
                            printf("Type in ID arglist %d\n and type=%d\n",$1->type,type);
                            $$=createTree(-1,$1->type,$1->varname,tFunction,$3,NULL,NULL);
                        };
    ArgList:ArgList COMMA expr 
                        {
                            /* printf("The type=%d\n",$3->type); */
                            struct typi*temp=(struct typi*)malloc(sizeof(struct typi));
                            temp->type=$3->type;
                            temp->next=typ;
                            typ=temp;
                            $1->third=$3;
                            $$=$1;
                        }
        | expr
                        {
                            /* printf("The type=%d\n",$1->type); */
                            struct typi*temp=(struct typi*)malloc(sizeof(struct typi));
                            temp->type=$1->type;
                            temp->next=typ;
                            typ=temp;
                            $$=$1;
                        };
    VAR: ID
                        {
                        if(LLookup($1->varname)==NULL && Lookup($1->varname)==NULL)
                        {
                            printf("Variable '%s' not declared!", $1->varname);
                            exit(1);
                        }
                        if(LLookup($1->varname)!=NULL)
                        {
                            $1->Gentry=NULL;
                            struct Lsymbol*temp=LLookup($1->varname);
                            $1->type=temp->type;
                        }
                        else {
                        $1->Gentry=Lookup($1->varname);
                        $1->type=$1->Gentry->type;
                        }
                            $$=$1;
                        };
        |ID '[' expr ']'
                        {
                            if($3->type!=tINT)
                            {
                                printf("array index is not int");
                                exit(1);
                            }
                            if(LLookup($1->varname)==NULL && Lookup($1->varname)==NULL)
                            {
                                printf("array not %s declared",$1->varname);
                                exit(1);
                            }
                            if(LLookup($1->varname)!=NULL)
                            {
                               $1->Gentry=NULL;
                                $1->type=type;
                            }
                            else {
                            $1->Gentry=Lookup($1->varname);
                            $1->type=$1->Gentry->type;
                            }
                            $$=createTree(-1,$1->type,NULL,tARRAY,$1,$3,NULL);
                        };
    %%
int yyerror(char const *s)
{
    printf("yyerror %s",s);
    return 0;
}


int main() {
  targetFile=fopen("targetFile.xsm","w");
  if(targetFile==NULL)
  {
    printf("file error\n");
  }
  yyin=fopen("input_l","r");
  yyparse();
  if(targetFile!=NULL)
  fclose(targetFile);
  return 0;
}
