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

#ifndef __PARSEJSONTYPES__
#define __PARSEJSONTYPES__

#include <stdbool.h>
#include <errno.h>

#define MAX_NODE_STACK 1024
#define NO_CONFIG_FILE_ERR -1
#define NO_PARSE_ERR -2

#define WRONG_NODE_TYPE		1
#define WRONG_INITIAL_NODE	2
#define UNHANDLED_NODE_TYPE	3
#define NO_MORE_ELEMENTS	4

#define MAX_NAME_SIZE 1024
#define MAX_INDEX_SIZE 10240

#define ERRSTREAM stderr

typedef enum {MOVE_INNER_OBJECT, MOVE_INNER_ARRAY, DONT_CHANGE_BRANCH, END_OBJ_PENDING, END_OBJ_CLOSED} branch;
#define CLOSED_ALLOCATION -2
typedef enum {ROOT_T, EMPTY_T, NUMERIC_T, TEXT_T, BOOL_T, MATRIX_T, OBJ_T, UNASSIGNED_T} type;

#define ROOT_NODE_NAME "root"

typedef union configData{
	double number;
	char* string;
	bool boolean;
} configData;

typedef struct node node;

struct node{
	char* nodeName;
	type nodeType;
	configData data;
	node* previousNode;
	node* nextNode;
	node* innerNode;
	node* outerNode;
	node* exitNode;
};

typedef struct pathIndex pathIndex;

struct pathIndex{
        char* fullNodeName;
        node* Node;
};

typedef struct pathBuffer pathBuffer;

struct pathBuffer{
        char** nodePathComponent;
        node* Node;
        pathBuffer* Next;
};

void addNode(char* nodeName);
node* allocateEmptyNode(void);
void makeLinks(node* currentNode, node* newNode,type objType);
void initObjName(node* newNode, type objType);
void makePathChain(node* objOrArray);
int createIndex(pathIndex** fullIndexesList);
int indexesListCmp (const void* sbuff1, const void* sbuff2);
void memoryError(void);
#define checkMemoryErrors(X) if(X == NULL) memoryError()
void spaceError(char* errMsg);
#define chackSpace(X,Y,Z) if(X == Y) spaceError(Z)
/*@null@*/
pathIndex* getElementValueByString(pathIndex* fullIndexesList, char* key);
bool checkNodeType(node* nodeElement, type expectType);
node* getArrayElements(node* arrayRootElement);
node* getArrayOfObjsElements(node* arrayRootElement);
#define verbose(X,Y) if(X) fprintf(ERRSTREAM, Y);
#define verboseLong(X,Y,Z) if(X) fprintf(ERRSTREAM, Y, Z);
bool isScalar(node* toCheck);
node* getElementFromObj(node* object);

#endif

