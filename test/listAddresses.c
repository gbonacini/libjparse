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

#include <stdio.h>
#include <stdlib.h>
#include <parseJsonConfig.h>

int printNodes(node* nodeElement, int node);

int main(int argc, char** argv){

  node nodeRoot;
  node* nodeCurrent;
  int result=0, node=0;

  pathIndex* fullIndexesList;

 if(argc < 2){
		fprintf(stderr,"Test file path needed!");
		exit(1);
 }
 
 result=parseJsonConfig(argv[1], &nodeRoot, &fullIndexesList);

 if(result<=0){
	fprintf(stderr,"Parse status: NOK Err: %d\n", result);
	exit(-1);
 }

 nodeCurrent=&nodeRoot;

 while(nodeCurrent!=NULL){
		node++;
		if(printNodes(nodeCurrent, node) == 0){
			if(nodeCurrent->innerNode!=NULL)
				nodeCurrent=nodeCurrent->innerNode;
			else if(nodeCurrent->nextNode!=NULL)
				nodeCurrent=nodeCurrent->nextNode;
			else
				nodeCurrent=nodeCurrent->exitNode;
		}else{
			fprintf(stderr,"Print status: NOK Err: %d\n", result);
			exit(-1);
		}

 }

 return 0;
}

int printNodes(node* nodeElement, int node){
		int err=0;
		if(nodeElement != NULL){
			fprintf(stderr,"NODE;%d;Label;%s;",node ,nodeElement->nodeName);
			switch(nodeElement->nodeType){
				case ROOT_T:
					fprintf(stderr,"Type;ROOT_T;Value;0;");
				break;
				case EMPTY_T:
					fprintf(stderr,"Type;EMPTY_T;Value;0;");
				break;
				case  NUMERIC_T:
					fprintf(stderr,"Type;NUMERIC_T;Value;%f;",(nodeElement->data).number);
				break;
				case  TEXT_T:
					fprintf(stderr,"Type;TEXT_T;Value;%s;",(nodeElement->data).string);
				break;
				case  BOOL_T:
					fprintf(stderr,"Type;BOOL_T;Value;%d;",(nodeElement->data).boolean);
				break;
				case  MATRIX_T:
					fprintf(stderr,"Type;MATRIX_T;Value;0;");
				break;
				case  OBJ_T:
					fprintf(stderr,"Type;OBJ_T;Value;0;");
				break;
				case UNASSIGNED_T:
					fprintf(stderr,"Type;UNASSIGNED;Type;%d;",nodeElement->nodeType);
				break;
			}
			fprintf(stderr,"node;%p;previousNode;%p;nextNode;%p;innerNode;%p;outerNode;%p;exitNode;%p\n",\
				nodeElement, nodeElement->previousNode, nodeElement->nextNode,\
				nodeElement->innerNode, nodeElement->outerNode, nodeElement->exitNode );
		}else{
			fprintf(stderr,"NODE -- Null node !");
			err=-1;
		}
		return err;
}
