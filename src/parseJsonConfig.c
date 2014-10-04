#include <jsonParserTypes.h>
#include <stdlib.h>
#include <string.h>

#ifndef __STANDALONE_MAKEFILE__
#include <jsonGram.h>
#else
#include <y.tab.h>
#endif

#include <stdio.h>

extern FILE * yyin;
extern int yydebug;
extern int yyparse(node* nodeZero);
extern int list;
extern pathBuffer* pathList[MAX_NODE_STACK];

int parseJsonConfig(char* configPath, node* nodeRoot, pathIndex** fullIndexesList){
	FILE* CONFIG= fopen(configPath, "r");
	if(CONFIG != NULL){
		yyin=CONFIG;
		yydebug=1;
		if(yyparse(nodeRoot)!= 0){
			return NO_PARSE_ERR;
		}
	}else{
		return NO_CONFIG_FILE_ERR;
	}
	(void)fclose(CONFIG);
	return createIndex(fullIndexesList);
}

int createIndex(pathIndex** fullIndexesList){
        int index=0, errstate=list;
        char buffer[MAX_INDEX_SIZE];
        pathBuffer* currentPath=NULL;
        pathBuffer* lastPath=NULL;
	node* lastPNode=NULL;

        #ifdef __DEBUG__JSON__
                fprintf(stderr,"Lex - Start  yywrap()\n");  
        #endif

        *fullIndexesList=(pathIndex*) calloc ((size_t)list, sizeof(pathIndex));
	if( fullIndexesList == NULL){
		memoryError();
	}

        for(index=0;index<list;index++){
                memset(buffer, 0, sizeof(buffer));
                currentPath=pathList[index];

                while(currentPath!=NULL ){
                        if( (MAX_INDEX_SIZE - ( 3 + strlen(buffer))) > strlen(*currentPath->nodePathComponent)){
                                strcat(buffer, *currentPath->nodePathComponent);
                                if(currentPath->Next!=NULL){
                                        strcat(buffer, ".");
                                }   
                        }else{
                                memset(buffer, 0, sizeof(buffer));
                                strcat(buffer, "String too long");
				errstate=-1;
                                break;
                        }   
			lastPath=currentPath;
			lastPNode=lastPath->Node;
                        currentPath=currentPath->Next;
			free(lastPath);
			lastPath=NULL;
                }   

                #ifdef __DEBUG__JSON__
                        fprintf(stderr,"Lex - Composed Path -- %s -- step -- %d\n", buffer, index);  
                #endif
		free(currentPath);
		currentPath=NULL;
                (*fullIndexesList + index)->fullNodeName=strdup(buffer);
                (*fullIndexesList + index)->Node=lastPNode; 

		pathList[index]=NULL;
        }   

        qsort (*fullIndexesList, (size_t)list, sizeof(pathIndex), indexesListCmp);

	return errstate;
}

int indexesListCmp (const void* sbuff1, const void* sbuff2){
  return strcmp (((pathIndex*)sbuff1)->fullNodeName, ((pathIndex*)sbuff2)->fullNodeName);
}

int indexesListSearch (const void* sbuff1, const void* sbuff2){
  #ifdef __DEBUG__JSON__
     fprintf(stderr,"Lex - Compare -- %s -- step -- %s\n", (char*)sbuff1, ((pathIndex*)sbuff2)->fullNodeName);  
  #endif
  return strcmp ((char*)sbuff1, ((pathIndex*)sbuff2)->fullNodeName);
}

/*@null@*/
pathIndex* getElementValueByString(pathIndex* fullIndexesList, char* key){
	return bsearch( (void*)key, fullIndexesList, (size_t)list, sizeof (pathIndex), indexesListSearch);
}

bool checkNodeType(node* nodeElement, type expectType){
        if(nodeElement->nodeType == expectType)
                        return true;
        return false;
}

node* getArrayElements(node* arrayRootElement){
	static node* currentNode;
	errno=0;

	if(arrayRootElement == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(!isScalar(currentNode) || errno!=0 ){
				      errno=UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=WRONG_INITIAL_NODE;
		}
	}else{
		if(arrayRootElement->nodeType==MATRIX_T){
			currentNode=arrayRootElement->innerNode;
			if(!isScalar(currentNode) || errno!=0 ){
			      errno=UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}


node* getArrayOfObjsElements(node* arrayRootElement){
	static node* currentNode;
	errno=0;

	if(arrayRootElement == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(currentNode->nodeType != OBJ_T){
				      errno=UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=WRONG_INITIAL_NODE;
		}
	}else{
		if(arrayRootElement->nodeType==MATRIX_T){
			currentNode=arrayRootElement->innerNode;
			if(currentNode==NULL){
				currentNode=arrayRootElement->nextNode;
			}
			if(currentNode->nodeType != OBJ_T){
			      errno=UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}

node* getElementFromObj(node* object){
	static node* currentNode;
	errno=0;

	if(arrayRootElement == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(!isScalar(currentNode) || errno!=0){
				      errno=UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=WRONG_INITIAL_NODE;
		}
	}else{
		if(arrayRootElement->nodeType==OBJ_T){
			currentNode=arrayRootElement->innerNode;
			if(currentNode==NULL){
				currentNode=arrayRootElement->nextNode;
			}
			if(!isScalar(currentNode) || errno!=0){
			      errno=UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}

bool isScalar(node* toCheck){
	bool result=false;
	switch(toCheck->nodeType){
		case OBJ_T:
		case MATRIX_T:
		break;
		case NUMERIC_T:
		case TEXT_T:
		case BOOL_T:
		case EMPTY_T:
			result=true;
		break;
		default:
			errno=WRONG_NODE_TYPE;
	}
	return result;
}

