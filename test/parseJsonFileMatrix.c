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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <parseJsonConfig.h>

int printNodeValue(node* nodeStack);
__attribute__((noreturn)) void printErrorMesg(char* msg);

static bool checktype=false, debug=false;

int main(int argc, char** argv){

  char jsonFile[MAX_NAME_SIZE];
  char url[MAX_NAME_SIZE];

  int index=0;

  struct stat statBuffer;

  bool fileName=false, value=false;
  node nodeRoot;
  node* tempNode;
  int result=0, c=0;

  pathIndex* fullIndexesList;
  pathIndex* element;

  while ( (c = getopt(argc, argv, "df:v:ht")) != EOF) {
	switch (c) {
	case 'd':               /* Debug switch  */
		debug=true;
	break;
	case 'f':               /* Json File  */
		fileName=true;
		memset(jsonFile,0,MAX_NAME_SIZE);
		strncpy(jsonFile, optarg, MAX_NAME_SIZE-1);
		if(stat(jsonFile, &statBuffer)){
			fprintf(ERRSTREAM, "The file %s do not exist!", jsonFile);
			exit (1);
		}
	break;
	case 'h':               /* Help */
			printErrorMesg(NULL); 
			// The program exits
	case 't':               /* Type */
			checktype=true;
	break;
	case 'v':               /* Value */
		value=true;
		memset(url,0,MAX_NAME_SIZE);
		strncpy(url, optarg, MAX_NAME_SIZE-1);
	break;
	case '?':		/* Default */
			printErrorMesg(NULL);

	}
 }

 if(!fileName){
		printErrorMesg("The json file was not specified.!\n");
 }
 if(!value){
		printErrorMesg("The url was not specified.!\n");
 }
 
 result=parseJsonConfig((char const *)jsonFile, &nodeRoot, &fullIndexesList);

 if(result>0){
	verboseLong(debug, "Parse status: OK IndexLen: %d\n", result);
 }else{
	verboseLong(debug, "Parse status: NOK Err: %d\n", result);
	exit(-1);
 }



 element=getElementValueByString(fullIndexesList, (char const *)url); 
 verbose(debug, "------------- End Search ---------------\n");

 tempNode=getArrayOfObjsElements(element->Node);
 if(tempNode!=NULL){
	tempNode=getElementFromObj(tempNode);
	fprintf(ERRSTREAM, "%d:", index);
	printNodeValue(tempNode);
	while((tempNode=getElementFromObj(NULL))){
		printNodeValue(tempNode);
	}
	fprintf(ERRSTREAM, "\n");
	index++;
 }

 while((tempNode=getArrayOfObjsElements(NULL))){
	 if(tempNode!=NULL){
		tempNode=getElementFromObj(tempNode);
		fprintf(ERRSTREAM, "%d:", index);
		printNodeValue(tempNode);
		while((tempNode=getElementFromObj(NULL))){
			printNodeValue(tempNode);
		}
		fprintf(ERRSTREAM, "\n");
		index++;
	 }
 }


 verbose(debug, "------------- End Result  ---------------\n");

 return 0;
}

void printErrorMesg(char* msg){
	fprintf(ERRSTREAM, "%s", msg);
	if(msg!=NULL)
	fprintf(ERRSTREAM, "%s%s%s%s%s","Usage:\n -f<Json File> \t\t To set the Json file to process.\n",\
					" -v<Value> \t\t To set the value to search.\n",\
					" -d \t\t To print verbose tracing messages.\n",\
					" -t \t\t To print the type of the value.\n",\
					" -h \t\t To print this help message.\n");
	exit(1);
}

int printNodeValue(node* nodeElement){
			int err=0;
			node* currNode=NULL;

			if(nodeElement != NULL){
				verboseLong(debug,"NODE -- Label:%s -- ",nodeElement->nodeName);
				if(debug) 
					fprintf(ERRSTREAM,"node: %p - previousNode: %p - nextNode: %p - innerNode: %p - outerNode: %p\n",\
						nodeElement, nodeElement->previousNode, nodeElement->nextNode,\
						nodeElement->innerNode, nodeElement->outerNode );
				switch(nodeElement->nodeType){
					case ROOT_T:
						if(checktype)
							fprintf(ERRSTREAM," This is the ROOT node.\nValue: \n");
					break;
					case EMPTY_T:
						if(checktype)
							fprintf(ERRSTREAM," EMPTY_T is present.\n");
							fprintf(ERRSTREAM,"<null>:");
					break;
					case  NUMERIC_T:
						if(checktype)
							fprintf(ERRSTREAM," NUMERIC_T is present.\nValue: \n");
						fprintf(ERRSTREAM,"%f:",(nodeElement->data).number);
					break;
					case  TEXT_T:
						if(checktype)
							fprintf(ERRSTREAM," TEXT_T is present.\nValue: \n");
						fprintf(ERRSTREAM,"%s:",(nodeElement->data).string);
					break;
					case  BOOL_T:
						if(checktype)
							fprintf(ERRSTREAM," BOOL_T is present.\nValue: \n");
						fprintf(ERRSTREAM,"%d:",(nodeElement->data).boolean);
					break;
					case  MATRIX_T:
						if(checktype)
							fprintf(ERRSTREAM," MATRIX_T is present.\n");
						currNode=nodeElement->innerNode;
						while(currNode!=NULL){
							if(currNode->innerNode!=NULL)
								currNode=currNode->innerNode;
							else if(currNode->nextNode!=NULL)
								currNode=currNode->nextNode;
							else
								currNode=NULL;
						}
					break;
					case  OBJ_T:
						fprintf(ERRSTREAM," OBJ_T is present \n ");
					break;
					case UNASSIGNED_T:
						fprintf(ERRSTREAM," Error: UNASSIGNED is present. ");
						fprintf(ERRSTREAM," Type:%d \n ",nodeElement->nodeType);
					break;
				}
                        }else{
				fprintf(ERRSTREAM,"<null>:");
				err=-1;
			}
			return err;
}
