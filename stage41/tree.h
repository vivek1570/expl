#ifndef TREE_H
#define TREE_H  

#include<stdio.h>


#define tNUM 0
#define tVAR 1
#define tADD 2
#define tSUB 3
#define tMUL 4
#define tDIV 5
#define tREAD 6
#define tWRITE 7
#define tASSIGN 8
#define tCONNECT 9
#define tINT 10
#define tIF 11
#define tWHILE 12
#define tBOOL 13
#define tLT 14
#define tLE 15
#define tGT 16
#define tGE 17
#define tEQ 18
#define tNE 19
#define tCONTINUE 20
#define tBREAK 21

#define VARINT 1
#define VARSTR 2

#define VARLEN 10

typedef struct tnode {
    int val;        // value of a number for NUM nodes.
    int type;       // type of variable
    char* varname;  // name of a variable for ID nodes
    int nodetype;   // information about non-leaf nodes - read/write/connector/+/* etc.
    struct Gsymbol *Gentry;     // pointer to GST entry for global variables and functions
    struct tnode *left,*right,*third;  // left and right branches
}tnode;



// struct Gsymbol *Lookup(char * name);            // Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
// void Install(char *name, int type, int size);   // Creates a symbol table entry.




/*Create a node tnode*/
int getReg();
void freeReg();
int getLabel();

struct tnode* createTree(int val, int type, char* c,int nodetype, struct tnode *l, struct tnode *r,struct tnode*t);

int codeGen(FILE *fp,struct tnode*root);

void printRead(FILE* fp,int addr);

void printWrite(FILE* fp,int regNum);

int getVaraddr(struct tnode*root);

void prefix(struct tnode*root);


#endif