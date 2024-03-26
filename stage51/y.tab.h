/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    PLUS = 259,                    /* PLUS  */
    MINUS = 260,                   /* MINUS  */
    MUL = 261,                     /* MUL  */
    DIV = 262,                     /* DIV  */
    WRITE = 263,                   /* WRITE  */
    READ = 264,                    /* READ  */
    EOL = 265,                     /* EOL  */
    ASSIGN = 266,                  /* ASSIGN  */
    ID = 267,                      /* ID  */
    IF = 268,                      /* IF  */
    then = 269,                    /* then  */
    Else = 270,                    /* Else  */
    Return = 271,                  /* Return  */
    LE = 272,                      /* LE  */
    GE = 273,                      /* GE  */
    LT = 274,                      /* LT  */
    GT = 275,                      /* GT  */
    NE = 276,                      /* NE  */
    EQ = 277,                      /* EQ  */
    MSG = 278,                     /* MSG  */
    MOD = 279,                     /* MOD  */
    DECL = 280,                    /* DECL  */
    ENDDECL = 281,                 /* ENDDECL  */
    INT = 282,                     /* INT  */
    STR = 283,                     /* STR  */
    COMMA = 284,                   /* COMMA  */
    MAIN = 285,                    /* MAIN  */
    START = 286,                   /* START  */
    END = 287,                     /* END  */
    ENDIF = 288,                   /* ENDIF  */
    endwhile = 289,                /* endwhile  */
    WHILE = 290,                   /* WHILE  */
    DO = 291,                      /* DO  */
    BREAK = 292,                   /* BREAK  */
    CONTINUE = 293                 /* CONTINUE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NUM 258
#define PLUS 259
#define MINUS 260
#define MUL 261
#define DIV 262
#define WRITE 263
#define READ 264
#define EOL 265
#define ASSIGN 266
#define ID 267
#define IF 268
#define then 269
#define Else 270
#define Return 271
#define LE 272
#define GE 273
#define LT 274
#define GT 275
#define NE 276
#define EQ 277
#define MSG 278
#define MOD 279
#define DECL 280
#define ENDDECL 281
#define INT 282
#define STR 283
#define COMMA 284
#define MAIN 285
#define START 286
#define END 287
#define ENDIF 288
#define endwhile 289
#define WHILE 290
#define DO 291
#define BREAK 292
#define CONTINUE 293

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "tree.y"

        struct tnode *node;
    

#line 148 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
