#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/shared.h"

int main(int argc, char *argv[])
{
    char **sortArgumentArray = (char**)malloc(15*sizeof(char*));
	for(int i = 0; i < 15; i++)
		sortArgumentArray[i] = (char*)malloc(100*sizeof(char));
    
    sortArgumentArray[0] = "sort";
    sortArgumentArray[1] = argv[1];
    sortArgumentArray[2] = NULL;
    printf("FILEEEEE %s\n", sortArgumentArray[1]);

    FILE *fp;
    fp = fopen(argv[1],"r");

    execlp("sort", "output", (char*)NULL);
    
    fclose(fp);
    //execvp("/usr/bin/sort", sortArgumentArray);
    return 0;
}
