#include<string.h>
#include<stdio.h>
#include "tree.h"
#include "symbol.h"

int stackc=-1;
int stack1[20];
int stack2[20];
int stackb=-1;

int reg=0;
int label=0;

int getLabel()
{
    int curr=label;
    label++;
    return curr;
}

int getReg()
{
    int r=reg;
    reg++;
    if(reg>20)
    {
        printf("out of registers\n");
        exit(1);
    }
    return r;
}

void initial(FILE * fp)
{
    fprintf(fp,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nMOV SP,%d\n",0,2056,0,0,0,0,1,0,bind);
}
void endcode(FILE* fp)
{
    int r=getReg();
    fprintf(fp,"MOV R%d,\"Exit\"\nPUSH R%d\nCALL 0\nPOP R%d",r,r,r);
}

void freeReg()
{
    reg--;
    if(reg<0)
    {
        reg=0;
    }
}


struct tnode* createTree(int val, int type, char* c,int nodetype, struct tnode *l, struct tnode *r,struct tnode*t)
{
    switch(nodetype)
    {
        case tADD:
        case tSUB:
        case tMUL:
        case tDIV:
        if(l->type!=r->type || l->type!=tINT)
        {
            printf("\ntype mismatch error in add \n");
            exit(0);
        }
        else{
            type=tINT;
        }
        break;
        
        case tLE:
        case tGE:
        case tGT:
        case tLT:
        case tEQ:
        case tNE:
        if(l->type!=r->type ||l->type!=tINT)
        {
            printf("\ntype mismatch error in tLe ...\n");
            exit(0);
        }
        else type=tBOOL;
        break;
        case tASSIGN:
        if(l->type!=r->type)
        {
            printf("\ntype mismatch error in tassign\n");
            exit(0);
        }
        break;
        case tIF:
        case tWHILE:
        if(l->type!=tBOOL)
        {
            printf("\ntype mismatch error in tbool\n");
            exit(0);
        }
        break;

    }
    struct tnode* temp;
    temp=(struct tnode*)malloc(sizeof(struct tnode));    
    temp->val=val;
    temp->type=type;
    temp->varname=c;
    temp->nodetype=nodetype;
    temp->Gentry=NULL;
    temp->left=l;
    temp->right=r;
    temp->third=t;
}


void printRead(FILE* fp,int addr)
{
    int r=getReg();
    fprintf(fp,"MOV R%d,\"Read\"\nPUSH R%d\nMOV R%d,-1\nPUSH R%d\nMOV R%d,R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",r,r,r,r,r,addr,r,r,r,r,r,r,r,r);
    freeReg();
}
void printWrite(FILE* fp,int regNum)
{
    int r=getReg();
  fprintf(fp, "MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nMOV R%d,R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", r, r, r, r, r, regNum, r, r, r, r, r, r, r, r);
    freeReg();
}

int getVaraddr(struct tnode*root)
{
    if(root->Gentry==NULL)
        {
            printf("%s\n",root->varname);
            printf("In var Gentry have some errors in getVar\n");
            printf("The varname=%s\n",root->Gentry->name);
            return -1;
        }
    int varAddr=root->Gentry->binding;
    return  varAddr;
}

int codeGen(FILE*fp,struct tnode*root)
{
    int r,varAddr,sourceReg,destReg,label1,label2,label3,f;
    char* varname;
    if(root==NULL) return -1;
    switch (root->nodetype)
    {
    case tCONNECT:
        codeGen(fp,root->left);
        codeGen(fp,root->right);
        return -1;
    case tREAD:
        f=1;
        r=getReg();
        if(root->left==NULL)
        {
            printf("\nInvalid read statement\n");
            exit(0);
        }
        if(root->left->Gentry==NULL)
        {
            printf("In var Gentry have some errors in tREAD\n");
            return -1;
        }
        if(root->left->left)
        {
           if(root->left->left->nodetype==0)
            varAddr=root->left->Gentry->binding+root->left->left->val;
            else{
                f=0;
                int addr=root->left->left->Gentry->binding;
                varAddr=root->left->Gentry->binding;
                fprintf(fp,"MOV R%d,[%d]\n",r,addr);
                fprintf(fp,"ADD R%d,%d\n",r,varAddr);
            }
        }
        else varAddr=root->left->Gentry->binding;
        if(f)
        fprintf(fp,"MOV R%d,%d\n",r,varAddr);
        printRead(fp,r);
        freeReg();
        return -1;
    case tWRITE:
    int r=codeGen(fp,root->left);
    printWrite(fp,r);
    return -1;
    case tNUM:
     r=getReg();
     fprintf(fp,"MOV R%d,%d\n",r,root->val);
     return r;
    case tMSG:
        r=getReg();
        fprintf(fp,"MOV R%d,%s\n",r,root->varname);
        return r;
    case tVAR:
    r=getReg();
    f=1;
    if(root->left)
    {
        if(root->left->nodetype==0)
        varAddr=root->Gentry->binding+root->left->val;
        else{
            f=0;
            varAddr=root->Gentry->binding;
            int addr=root->left->Gentry->binding;
            int t=getReg();
            fprintf(fp,"MOV R%d,[%d]\n",t,addr);
            fprintf(fp,"ADD R%d,%d\n",t,varAddr);
            fprintf(fp,"MOV R%d,[R%d]\n",r,t);
            freeReg();
        }
    }
    else varAddr=root->Gentry->binding;
    if(f)
    fprintf(fp,"MOV R%d,[%d]\n",r,varAddr);
    return r;
    case tADD:
      sourceReg = codeGen(fp, root->left);
      destReg = codeGen(fp, root->right);
      fprintf(fp, "ADD R%d,R%d\n", sourceReg, destReg);
      freeReg();
      return sourceReg;
    case tSUB:
      sourceReg = codeGen(fp, root->left);
      destReg = codeGen(fp, root->right);
      fprintf(fp, "SUB R%d,R%d\n", sourceReg, destReg);
      freeReg();
      return sourceReg;
    case tMUL:
      sourceReg = codeGen(fp, root->left);
      destReg = codeGen(fp, root->right);
      fprintf(fp, "MUL R%d,R%d\n", sourceReg, destReg);
      freeReg();
      return sourceReg;
    case tDIV:
      sourceReg = codeGen(fp, root->left);
      destReg = codeGen(fp, root->right);
      fprintf(fp, "DIV R%d,R%d\n", sourceReg, destReg);
      freeReg();
      return sourceReg;
    case tGE:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"GE R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tGT:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"GT R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tLE:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"LE R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tLT:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"LT R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tNE:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"NE R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tEQ:
        sourceReg=codeGen(fp,root->left);
        destReg=codeGen(fp,root->right);
        fprintf(fp,"EQ R%d,R%d\n",sourceReg,destReg);
        freeReg();
        return sourceReg;
    case tASSIGN:
        f=1;
        if(root->left->left)
        {
            if(root->left->left->nodetype==0)
            varAddr=root->left->Gentry->binding+root->left->left->val;
            else{
                f=0;
                varAddr=root->left->Gentry->binding;
                int addr=root->left->left->Gentry->binding;
                r=getReg();
                fprintf(fp,"MOV R%d,[%d]\n",r,addr);
                fprintf(fp,"ADD R%d,%d\n",r,varAddr);
                destReg=codeGen(fp,root->right);
                fprintf(fp,"MOV [R%d],R%d\n",r,destReg);
                freeReg();
            }
        }
        else varAddr=root->left->Gentry->binding;
        // int assaddr=getVaraddr(root->left);
        if(f)
        {
        destReg=codeGen(fp,root->right);
        fprintf(fp,"MOV [%d],R%d\n",varAddr,destReg);
        }
        freeReg();
        return -1;
    case tIF:
        label1=getLabel();
        if(root->third!=NULL)
        label2=getLabel();
        // fprintf(fp,"L%d:\n",label1);
        sourceReg=codeGen(fp,root->left);
        if(root->third!=NULL)
        fprintf(fp,"JZ R%d,L%d\n",sourceReg,label2);
        else
        fprintf(fp,"JZ R%d,L%d\n",sourceReg,label1);

        codeGen(fp,root->right);
        fprintf(fp,"JMP L%d\n",label1);

        if(root->third!=NULL)
        {
            fprintf(fp,"L%d:\n",label2);
            codeGen(fp,root->third);
            fprintf(fp,"JMP L%d\n",label1);
        }

        fprintf(fp,"L%d:\n",label1);
        return -1;
    case tWHILE:
        label1=getLabel();
        label2=getLabel();
        stackb++;
        stackc++;
        stack1[stackc]=label1;
        stack2[stackb]=label2;
        fprintf(fp,"L%d:\n",label1);
        sourceReg=codeGen(fp,root->left);
        fprintf(fp,"JZ R%d,L%d\n",sourceReg,label2);
        codeGen(fp,root->right);
        fprintf(fp,"JMP L%d\n",label1);
        fprintf(fp,"L%d:\n",label2);
        stackb--;
        stackc--;
        return -1;
    case tCONTINUE:
        if(stackc<0)
        {
            printf("Some errors has detected in tCONTINUE\n");
            exit(0);
        }
        printf("stackc=%d and value=%d\n",stackc,stack1[stackc]);
        fprintf(fp,"JMP L%d\n",stack1[stackc]);
        return -1;
    case tBREAK:
        if(stackb<0)
        {
            printf("Some errors have been in tBreak\n");
            exit(0);
        }
        fprintf(fp,"JMP L%d\n",stack2[stackb]);
        return -1;
    default:
    printf("\ncode gen have some errors %d\n",root->nodetype);
    exit(0);
    }
}
void prefix(struct tnode*root)
{
    if(root!=NULL)
    {
        printf("%d ",root->nodetype);
        // if(root->nodetype==tMSG)
        // {
        //     printf("str=%s\n",root->varname);
        // }
        // if(root->nodetype==tNUM)
        // printf("val=%d\n",root->val);
        prefix(root->left);
        prefix(root->right);
    }
}




