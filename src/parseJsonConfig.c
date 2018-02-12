// -----------------------------------------------------------------
// Libjparse - A JSON configuration files parser.
// Copyright (C) 2014  Gabriele Bonacini
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// -----------------------------------------------------------------

#include <jsonParserTypes.h>
#include <parseJsonConfig.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int parseJsonConfig(char const *configPath, node* nodeRoot, pathIndex** fullIndexesList){
	FILE* CONFIG= fopen(configPath, "r");
	if(CONFIG != NULL){
		yyin=CONFIG;
		yydebug=1;
		if(yyparse(nodeRoot)!= 0){
			return ERR_NO_PARSE;
		}
	}else{
		return ERR_NO_CONFIG_FILE;
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
pathIndex* getElementValueByString(const pathIndex * const fullIndexesList, char const * const key){
	return bsearch( (void*)key, fullIndexesList, (size_t)list, sizeof (pathIndex), indexesListSearch);
}

bool checkNodeType(const node * const nodeElement, const type expectType){
        if(nodeElement->nodeType == expectType)
                        return true;
        return false;
}

node* getArrayElements(const node * const arrayRootElement){
	static node* currentNode;
	errno=0;

	if(arrayRootElement == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(!isScalar(currentNode) || errno!=0 ){
				      errno=ERR_UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=ERR_NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=ERR_WRONG_INITIAL_NODE;
		}
	}else{
		if(arrayRootElement->nodeType==MATRIX_T){
			currentNode=arrayRootElement->innerNode;
			if(!isScalar(currentNode) || errno!=0 ){
			      errno=ERR_UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=ERR_WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}


node* getArrayOfObjsElements(const node * const arrayRootElement){
	static node* currentNode;
	errno=0;

	if(arrayRootElement == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(currentNode->nodeType != OBJ_T){
				      errno=ERR_UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=ERR_NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=ERR_WRONG_INITIAL_NODE;
		}
	}else{
		if(arrayRootElement->nodeType==MATRIX_T){
			currentNode=arrayRootElement->innerNode;
			if(currentNode==NULL){
				currentNode=arrayRootElement->nextNode;
			}
			if(currentNode->nodeType != OBJ_T){
			      errno=ERR_UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=ERR_WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}

node* getElementFromObj(const node * const object){
	static node* currentNode;
	errno=0;

	if(object == NULL){
		if(currentNode!=NULL){
			if(currentNode->nextNode!=NULL){
				currentNode=currentNode->nextNode;
				if(!isScalar(currentNode) || errno!=0){
				      errno=ERR_UNHANDLED_NODE_TYPE;
				      currentNode=NULL;
				}
			}else{
				errno=ERR_NO_MORE_ELEMENTS;
				currentNode=NULL;
			}
		}else{
			errno=ERR_WRONG_INITIAL_NODE;
		}
	}else{
		if(object->nodeType==OBJ_T){
			currentNode=object->innerNode;
			if(currentNode==NULL){
				currentNode=object->nextNode;
			}
			if(!isScalar(currentNode) || errno!=0){
			      errno=ERR_UNHANDLED_NODE_TYPE;
			      currentNode=NULL;
			}
				
		}else{
			errno=ERR_WRONG_NODE_TYPE;
			currentNode=NULL;
		}
	} 
	
	return currentNode;
}

bool isScalar(const node * const toCheck){
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
		case ROOT_T:
		case UNASSIGNED_T:
			errno=ERR_WRONG_NODE_TYPE;
	}
	return result;
}

