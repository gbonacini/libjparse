#ifndef __JSON_GRAM_TYPES__
#define __JSON_GRAM_TYPES__

#include <jsonGram.h>

extern int level;
extern int id;
extern branch moveBranch;
extern int yyerror();
extern node* currentNode;
extern node* nodeStack[MAX_NODE_STACK];

#endif
