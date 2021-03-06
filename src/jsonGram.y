%{
#include <stdio.h>
#include <stdlib.h>
#include <jsonParserTypes.h>
#include <string.h>

int yylex(void);
int yyerror (node* nodeZero, const char *msg);

branch moveBranch;

int yydebug=0;
int id=0, level=0, list=0;

node* currentNode=NULL;
node* nodeStack[MAX_NODE_STACK]; 
pathBuffer* pathList[MAX_NODE_STACK];

static int previousLevel=0;
static bool pendingClose=false;
static bool pendingEmptyObj=false;

static node* previousObject=NULL;
static node* pendingLinkInner=NULL;

static char objNameBuffer[MAX_NAME_SIZE];
static char** pathStack[MAX_NODE_STACK]; 

static pathBuffer* currentPathList=NULL;
static type currentType=UNASSIGNED_T;
static configData buffer;

%}

%start object

%union
{
        bool boolean;
        double number;
        char *string;
}

%token STARTBLOCK
%token ENDBLOCK
%token STARTARRAY
%token ENDARRAY
%token TAG
%token VOIDVALUE
%token SEPARATOR 
%token <boolean> BOOLEAN
%token <number> NUMBER
%token <string> STRING

%left VOIDVALUE

%error-verbose
%parse-param { node* nodeZero }

%initial-action
{
  currentNode=nodeZero;

  currentNode->nodeName=ROOT_NODE_NAME;
  currentNode->nodeType=ROOT_T;
  currentNode->previousNode=NULL;
  currentNode->nextNode=NULL;
  currentNode->innerNode=NULL;
  currentNode->outerNode=NULL;
};

%%


object:		emptyObject  	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Empty Object Detected\n") ;
				#endif
				;}
		;		
		|
		STARTBLOCK members ENDBLOCK 	{

							#ifdef __DEBUG__JSON__
								fprintf(ERRSTREAM, "YACC - Object -- Level %d\n", level) ;
							#endif
						;}
;

members:	expression  	{

					#ifdef __DEBUG__JSON__
						fprintf(ERRSTREAM, "YACC - Single Member \n") ;
					#endif
				;}
		;		
		|
		expression SEPARATOR members 	{

							#ifdef __DEBUG__JSON__
								fprintf(ERRSTREAM, "YACC - Members \n") ;
							#endif
						;}
;

expression:	STRING TAG value 	{
						#ifdef __DEBUG__JSON__
							fprintf(ERRSTREAM, "YACC - Expression: %s\n", $1);
						#endif

						switch(currentType){
							case NUMERIC_T:
								addNode($1);
								(currentNode->data).number=buffer.number;
								currentNode->nodeType=NUMERIC_T;
								makePathChain(NULL);
							break;
							case TEXT_T:
								addNode($1);
						
								(currentNode->data).string=buffer.string;
								currentNode->nodeType=TEXT_T;
								makePathChain(NULL);
							break;
							case BOOL_T:
								addNode($1);
						
								(currentNode->data).boolean=buffer.boolean;
								currentNode->nodeType=BOOL_T;
								makePathChain(NULL);
							break;
							case EMPTY_T:
								addNode($1);
						
								currentNode->nodeType=EMPTY_T;
								makePathChain(NULL);
							break;
							case MATRIX_T:
							case OBJ_T:
								pendingClose=true;
								free((nodeStack[level])->nodeName);
								(nodeStack[level])->nodeName=NULL;
								(nodeStack[level])->nodeName=$1;
								checkMemoryErrors((nodeStack[level])->nodeName);

								makePathChain(nodeStack[level]);
								#ifdef __DEBUG__JSON__
									fprintf(ERRSTREAM, "YACC - Node Upgraded Name Address %p\n",\
										(nodeStack[level])->nodeName);
								#endif
							break;
							case UNASSIGNED_T:
								currentNode->nodeType=UNASSIGNED_T;
							break;
							case ROOT_T:
								yyerror(NULL, "Unexpected Root Node");
						}
					;}
;

array:	emptyArray 	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Empty Array Case\n");
				#endif
			;}
	;
	|
	STARTARRAY elements ENDARRAY   	{

						#ifdef __DEBUG__JSON__
						fprintf(ERRSTREAM, "YACC - Expression Array Values List -- Level %d\n", level);
						#endif
					;}
;

elements:	value  	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Single Element \n") ;
				#endif
			;}
		;		
		|
		value SEPARATOR elements 	{

							#ifdef __DEBUG__JSON__
							fprintf(ERRSTREAM, "YACC - Elements \n") ;
							#endif
						;}
;

value:	NUMBER  	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value Number %f -- Level %d\n", $1, level) ;
				#endif

				currentType=NUMERIC_T;
				buffer.number=$1;
			;}
	;
	|
	STRING  	{
				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value String %s -- Level %d\n", $1, level) ;
				#endif
				currentType=TEXT_T;
				buffer.string=$1;
				checkMemoryErrors(buffer.string);
			;}
	;
	|
	BOOLEAN  	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value Bolean %d -- Level %d\n", $1, level);
				#endif
				currentType=BOOL_T;
				buffer.boolean=$1;
			;}
	;
	|
	VOIDVALUE 	{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value Null -- Level %d\n", level);
				#endif
				currentType=EMPTY_T;
			;}
	;
	|
	array 		{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value Array. \n");
				#endif
				currentType=MATRIX_T;
			;}
	;
	|
	object 		{

				#ifdef __DEBUG__JSON__
					fprintf(ERRSTREAM, "YACC - Value Object. \n");
				#endif
				currentType=OBJ_T;
			;}
;

emptyArray:	STARTARRAY  ENDARRAY		{

							#ifdef __DEBUG__JSON__
								fprintf(ERRSTREAM, "YACC - Empty Array -- Level %d\n", level);
							#endif
						;}
;
emptyObject:	STARTBLOCK  ENDBLOCK		{
							pendingEmptyObj=true;
							makePathChain(nodeStack[level]);
							#ifdef __DEBUG__JSON__
								fprintf(ERRSTREAM, "YACC - Empty Object -- Level %d\n", level);
							#endif
						;}
;

%%

/* Added because osx doesn't have liby.a installed. */
int yyerror (node* nodeZero, const char *msg) {
	return fprintf (ERRSTREAM, "YACC: Node %p -- error: %s\n",nodeZero,  msg);
}

void addNode(char* nodeName){
	node* newNode=NULL;

	switch(moveBranch){
		case MOVE_INNER_OBJECT:
			if(id!=0){
				newNode=allocateEmptyNode();

				makeLinks(currentNode, newNode, OBJ_T);
				initObjName(newNode, OBJ_T);

				pathStack[level]=&(newNode->nodeName);
				currentNode=newNode;	
				if(newNode->outerNode == newNode->previousNode)
					newNode->previousNode=NULL;
			}else{
				pathStack[level]=&(currentNode->nodeName);
			}

			pendingLinkInner=currentNode;
			previousObject=currentNode;
			previousLevel=level;
			id++;
		break;
		case MOVE_INNER_ARRAY:
			newNode=allocateEmptyNode();

			makeLinks(currentNode, newNode, MATRIX_T);
			initObjName(newNode, MATRIX_T);

			pathStack[level]=&(newNode->nodeName);

			currentNode=newNode;
			pendingLinkInner=currentNode;
				if(newNode->outerNode == newNode->previousNode)
						newNode->previousNode=NULL;
			id++;
		break;
		case DONT_CHANGE_BRANCH:
			newNode=allocateEmptyNode();

			makeLinks(currentNode, newNode, UNASSIGNED_T);

			if(newNode->outerNode == newNode->previousNode)
					newNode->previousNode=NULL;
			currentNode=newNode;	
		break;
		case END_OBJ_PENDING:
		case END_OBJ_CLOSED:
			#ifdef __DEBUG__JSON__
				fprintf(ERRSTREAM, "YACC - End obj pending or closed\n") ;
			#endif
		break;
	}
	if(nodeName!=NULL){
		currentNode->nodeName=nodeName;
		if(currentNode->nodeName==NULL){
			memoryError();
		}
	}
}

void makePathChain(node* objOrArray){
	pathList[list]=(pathBuffer*) calloc((size_t) 1, sizeof(pathBuffer));
	if(pathList[list]==NULL){
		memoryError();
	}
	currentPathList=pathList[list];

	int i;
	for(i=0; i<level; i++){
		currentPathList->nodePathComponent=pathStack[i];
		#ifdef __DEBUG__JSON__
			fprintf(ERRSTREAM, "YACC - Node Obj Address processed %p\n", *(pathStack[i]));
		#endif
		currentPathList->Next=\
			(pathBuffer*) calloc((size_t) 1, sizeof(pathBuffer));
		if(currentPathList->Next==NULL){
			memoryError();
		}
		currentPathList=currentPathList->Next;
	}

	if(objOrArray==NULL){
		currentPathList->nodePathComponent=&currentNode->nodeName;
		currentPathList->Node=currentNode;
	}else{
		currentPathList->nodePathComponent=&(nodeStack[level])->nodeName;
		currentPathList->Node=nodeStack[level];
	}

	currentPathList->Next=NULL;
	list++;
}

node* allocateEmptyNode(void){
	node* aNode=(node*) calloc((size_t) 1, sizeof(node));
	checkMemoryErrors(aNode);

	if(pendingLinkInner!=NULL){
		pendingLinkInner->nextNode=aNode;
		aNode->previousNode=pendingLinkInner;
		pendingLinkInner=NULL;
	}

	return aNode;
}

void makeLinks(node* currentNodePtr, node* newNode, type objType){
	if(currentNodePtr->nodeType == OBJ_T || currentNodePtr->nodeType == MATRIX_T){
		if(!pendingEmptyObj){
			currentNodePtr->innerNode=newNode;
			newNode->outerNode=currentNodePtr;
		}else{
			currentNodePtr->nextNode=newNode;
			newNode->previousNode=currentNodePtr;
			pendingEmptyObj=false;
		}
		if(currentNodePtr->innerNode == currentNodePtr->nextNode)
				currentNodePtr->nextNode=NULL;
		if(newNode->outerNode == newNode->previousNode)
				newNode->previousNode=NULL;
	}else if(objType == OBJ_T){
		currentNodePtr->exitNode=newNode;
		if(previousObject!=NULL && previousLevel==level){
			previousObject->nextNode=newNode;
			newNode->previousNode=previousObject;
		}else if(previousObject!=NULL && previousLevel>level){
			nodeStack[level]->nextNode=newNode;
		}
	}else if(objType == MATRIX_T){
		currentNodePtr->nextNode=newNode;
		newNode->previousNode=currentNodePtr;
	}else if(objType == UNASSIGNED_T){
		if(pendingClose){
			currentNodePtr->nextNode=NULL;
			currentNodePtr->exitNode=newNode;
			newNode->previousNode=nodeStack[level];
		}else{ 
			currentNodePtr->nextNode=newNode;
			newNode->previousNode=currentNodePtr;
			currentNodePtr->exitNode=NULL;
		}
		newNode->outerNode=nodeStack[level-1];
	}
}

void initObjName(node* newNode, type objType){
	memset(objNameBuffer, 0, sizeof(objNameBuffer));
	switch(objType){ 
		case OBJ_T:
			snprintf(objNameBuffer, MAX_NAME_SIZE-1, "Object_Data_%d", id);
			newNode->nodeType=OBJ_T;
		break;
		case MATRIX_T:
			snprintf(objNameBuffer, MAX_NAME_SIZE-1, "Array_Data_%d", id);
			newNode->nodeType=MATRIX_T;
		break;
		case ROOT_T:
		case EMPTY_T:
		case NUMERIC_T:
		case TEXT_T:
		case BOOL_T:
		case UNASSIGNED_T:
			// Nothing to do.
		break;
	}
	newNode->nodeName=strdup(objNameBuffer);
	checkMemoryErrors(newNode->nodeName);
}


void memoryError(void){
	yyerror(NULL, "Can't allocate memory.");

}

void spaceError(char* errMsg){
	yyerror(NULL, errMsg);
}
