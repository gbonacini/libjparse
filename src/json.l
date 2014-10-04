%option yywrap
%option nounput
%option noinput

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <jsonParserTypes.h>
#include <jsonGram.h>

extern int level;
extern int id;
extern branch moveBranch;
extern YYSTYPE yylval;
extern int yyerror();
extern node* currentNode;
extern node* nodeStack[MAX_NODE_STACK];

%}

%%

[ \t\n]+ /* Ignore Spaces */
[{] {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Start Block Token  -- ID: %d -- LEVEL -- %d\n", STARTBLOCK, level);  
	#endif

	moveBranch=MOVE_INNER_OBJECT;
	addNode(NULL);
	chackSpace(level, MAX_NODE_STACK, "Node Stack Static Space Not Sufficient.");
	nodeStack[level]=currentNode;
	level++;

	return STARTBLOCK;
}
[}] {
	moveBranch=END_OBJ_PENDING;
	addNode(NULL);
	level--;

	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - End Block Token  -- ID: %d -- LEVEL -- %d\n", ENDBLOCK, level);  
	#endif

	return ENDBLOCK;
}
[:] {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Tag Token -- %s  -- ID: %d\n", yytext, TAG);  
	#endif

	moveBranch=DONT_CHANGE_BRANCH;

	return TAG;
}
[-]?[0-9]+ {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Numeric Token -- %s  -- ID: %d\n", yytext, NUMBER);  
	#endif

	yylval.number=atof(yytext);
	moveBranch=DONT_CHANGE_BRANCH;

	return NUMBER;
}
"true"|"false" {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Boolean Token -- %s  -- ID: %d\n", yytext, BOOLEAN);  
	#endif

	if( strncmp(yytext,"true", sizeof("true")) == 0){ 
		yylval.boolean=true;
	}else{
		yylval.boolean=false;
	}
	moveBranch=DONT_CHANGE_BRANCH;

	return BOOLEAN;
}
null {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Null Token -- %s  -- ID: %d\n", yytext, VOIDVALUE);  
	#endif

	moveBranch=DONT_CHANGE_BRANCH;

	return VOIDVALUE;
}
["][^"]*["] {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - String Token -- %s -- ID: %d\n", yytext, STRING);  
	#endif

	yylval.string=strdup(yytext+1);
	checkMemoryErrors(yylval.string);
	yylval.string[strlen(yylval.string)-1]=0;
	moveBranch=DONT_CHANGE_BRANCH;

	return STRING;
}
\[ {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Start Array Token -- %s  -- ID: %d -- Level: %d\n", yytext, STARTARRAY, level);  
	#endif

	id=0;
	moveBranch=MOVE_INNER_ARRAY;
	addNode(NULL);
	chackSpace(level, MAX_NODE_STACK, "Node Stack Static Space Not Sufficient.");
	nodeStack[level]=currentNode;
	level++;

	return STARTARRAY;
}
\] {
	moveBranch=END_OBJ_PENDING;
	addNode(NULL);
	level--;

	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - End Array Token -- %s  -- ID: %d -- Level: %d\n", yytext, ENDARRAY, level);  
	#endif

	return ENDARRAY;
}
[,] {
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - Separtor Token -- %s  -- ID: %d\n", yytext, SEPARATOR);  
	#endif

	moveBranch=DONT_CHANGE_BRANCH;

	return SEPARATOR;
}
.  {
 yyerror("illegal token"); 
}
%%

int yywrap(){
	#ifdef __DEBUG__JSON__
 		fprintf(ERRSTREAM,"Lex - EOF\n");  
	#endif

	return 1; 
}

