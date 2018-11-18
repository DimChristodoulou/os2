#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h> 

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


int main(int argc, char *argv[])
{
    pid_t pid;
    printf("In rootNode with %d pid\n", getpid());
	// fifoPipe = "/tmp/fifo";
	// mkfifo(fifoPipe, 0666);
    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);
    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);
    int skew = atoi(argv[6]);
    printf("SKEW %d\n", skew);

    //printf("%d %d %d %s %s\n", height, startRead, endRead,fileName, patternName);

	char **argumentArray = (char**)malloc(9*sizeof(char*));
	
	for(int i = 0; i < 9; i++)
		argumentArray[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray[0] = "exe/splitter-merger";
	sprintf(argumentArray[1], "%d", height);
	sprintf(argumentArray[2], "%d", startRead);
	sprintf(argumentArray[3], "%d", endRead);
	argumentArray[4] = fileName;
	argumentArray[5] = patternName;
    argumentArray[6] = "";
    sprintf(argumentArray[7], "%d", skew);
	argumentArray[8] = NULL;

    pid = fork();
    if (pid == 0){
        execvp("exe/splitter-merger", argumentArray);
	}
    else if(pid < 0){
        perror("Error in forking");
    }
    else{
        wait(NULL);
        printf("ALL CHILDREN FINISHED\n");
    }
    return 0;
}
