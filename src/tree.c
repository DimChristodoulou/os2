#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "tree.h"
#include "shared.h"

FILE *dataFilePtr;

int strArraySearch(char **array, int len, char *delim){
	for(i = 0; i < len; ++i){
	    if(!strcmp(array[i], delim))
	        return i;
	}
	return -1;
}

int main(int argc, char const *argv[]){
	int count;

	int height;
	char *substr;

	if(argc == 8){
		count = strArraySearch(argv, argc, "-h");
		height = *argv[++count];

		count = strArraySearch(argv, argc, "-d");
		dataFilePtr = fopen(*argv[++count],"r");

		count = strArraySearch(argv, argc, "-p");
		strcpy( substr, *argv[++count] );
	}
	else if(argc == 1){
		height = 3;
		dataFilePtr = stdin;
		//CHANGE SUBSTR ACCORDING TO BINARY FILE
		substr = "test";
	}
	else{

	}

}