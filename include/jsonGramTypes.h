#ifndef __JSON_GRAM_TYPES__
#define __JSON_GRAM_TYPES__

/* Tokens.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STARTBLOCK = 258,
     ENDBLOCK = 259,
     STARTARRAY = 260,
     ENDARRAY = 261,
     TAG = 262,
     VOIDVALUE = 263,
     SEPARATOR = 264,
     BOOLEAN = 265,
     NUMBER = 266,
     STRING = 267
   };
#endif

/* Tokens.  */
#define STARTBLOCK 258
#define ENDBLOCK 259
#define STARTARRAY 260
#define ENDARRAY 261
#define TAG 262
#define VOIDVALUE 263
#define SEPARATOR 264
#define BOOLEAN 265
#define NUMBER 266
#define STRING 267

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 39 "/Users/bg/_Sorgenti/_Json/_Json_LY_hg/src/jsonGram.y"
{
        bool boolean;
        double number;
        char *string;
}
/* Line 1529 of yacc.c.  */
#line 79 "/Users/bg/_Sorgenti/_Json/_Json_LY_hg/src/../include/jsonGram.h"
	YYSTYPE;
#define yystype YYSTYPE /* obsolescent; will be withdrawn */
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern int level;
extern int id;
extern branch moveBranch;
extern int yyerror();
extern node* currentNode;
extern node* nodeStack[MAX_NODE_STACK];

#endif
