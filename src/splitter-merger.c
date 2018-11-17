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
#include "../inc/tree.h"

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
    char *pidStr = (char*)malloc(20*sizeof(char));
    int pid = getpid();
    sprintf(pidStr,"%d",pid);

    char *myfifo1 = (char*)malloc(50*sizeof(char));
    strcat(myfifo1, "tmp/fifo1");
    strcat(myfifo1, pidStr);
    if(mkfifo(myfifo1, 0666) < 0 ){
        perror("fifo failed!");
    } 

    char *myfifo2 = (char*)malloc(25*sizeof(char));
    strcat(myfifo2, "tmp/fifo2");
    strcat(myfifo2, pidStr);
    if(mkfifo(myfifo2, 0666) < 0 ){
        perror("fifo failed!");
    } 
    
    int fd1, fd2;

    char **argumentArray1 = (char**)malloc(8*sizeof(char*));
	for(int i = 0; i < 8; i++)
		argumentArray1[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray1[0] = "exe/splitter-merger";
	sprintf(argumentArray1[1], "%d", height-1);
	sprintf(argumentArray1[2], "%d", startRead);
	sprintf(argumentArray1[3], "%d", midRead);
	argumentArray1[4] = fileName;
	argumentArray1[5] = patternName;
    argumentArray1[6] = myfifo1;
	argumentArray1[7] = NULL;

    char **argumentArray2 = (char**)malloc(8*sizeof(char*));
	for(int i = 0; i < 8; i++)
		argumentArray2[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray2[0] = "exe/splitter-merger";
	sprintf(argumentArray2[1], "%d", height-1);
	sprintf(argumentArray2[2], "%d", midRead);
	sprintf(argumentArray2[3], "%d", endRead);
	argumentArray2[4] = fileName;
	argumentArray2[5] = patternName;
    argumentArray2[6] = myfifo2;
	argumentArray2[7] = NULL;
    
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
                //wait(NULL);
                MyRecord rec2;
                char temp2[50];
                printf("before open2\n");
                fd2 = open(myfifo2, O_RDONLY);
                printf("before read2\n");
                read(fd2, temp2, 50);
                printf("%s\n",temp2);
                //printf("%ld %s %s  %s %d %s %s %-9.2f\n", rec2.custid, rec2.LastName, rec2.FirstName, rec2.Street, rec2.HouseID, rec2.City, rec2.postcode, rec2.amount);
            }
            //wait(NULL);
            MyRecord rec1;
            char temp1[50];
            printf("before open1\n");
            fd1 = open(myfifo1, O_RDONLY);
            printf("before read1\n");
            read(fd1, temp1, 50);
            printf("%s\n",temp1);
            //printf("%ld %s %s  %s %d %s %s %-9.2f\n", rec1.custid, rec1.LastName, rec1.FirstName, rec1.Street, rec1.HouseID, rec1.City, rec1.postcode, rec1.amount);
        }
    }

    return 0;
}