#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h> 

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/shared.h"

int main(int argc, char const *argv[])
{
    printf("In splitter-merger with %d pid\n", getpid());

    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);
    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);

    int midRead = (startRead + endRead)/2;

    char **argumentArray1 = (char**)malloc(7*sizeof(char*));
	for(int i = 0; i < 7; i++)
		argumentArray1[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray1[0] = "exe/splitter-merger";
	sprintf(argumentArray1[1], "%d", height-1);
	sprintf(argumentArray1[2], "%d", startRead);
	sprintf(argumentArray1[3], "%d", midRead);
	argumentArray1[4] = fileName;
	argumentArray1[5] = patternName;
	argumentArray1[6] = NULL;

    char **argumentArray2 = (char**)malloc(7*sizeof(char*));
	for(int i = 0; i < 7; i++)
		argumentArray2[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray2[0] = "exe/splitter-merger";
	sprintf(argumentArray2[1], "%d", height-1);
	sprintf(argumentArray2[2], "%d", midRead);
	sprintf(argumentArray2[3], "%d", endRead);
	argumentArray2[4] = fileName;
	argumentArray2[5] = patternName;
	argumentArray2[6] = NULL;

    if(height > 1){
        pid_t pid1;
        pid1 = fork();
        if (pid1 == 0)
            execvp("exe/splitter-merger", argumentArray1);
        else if(pid1 < 0)
            perror("Error in forking");
        else{
            pid_t pid2;
            pid2 = fork();
            if (pid2 == 0)
            execvp("exe/splitter-merger", argumentArray2);
            else if(pid2 < 0)
                perror("Error in forking");
            else{
                //IN PARENT
                wait(NULL);
            }
            wait(NULL);
        }
    }
    else if(height == 1){
        pid_t pid1;
        pid1 = fork();
        if (pid1 == 0)
            execvp("exe/leafNode", argumentArray1);
        else if(pid1 < 0)
            perror("Error in forking");
        else{
            pid_t pid2;
            pid2 = fork();
            if (pid2 == 0)
            execvp("exe/leafNode", argumentArray2);
            else if(pid2 < 0)
                perror("Error in forking");
            else{
                //IN PARENT
                wait(NULL);
            }
            wait(NULL);
        }
    }

    return 0;
}