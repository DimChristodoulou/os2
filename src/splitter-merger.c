#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <math.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/shared.h"
#include "../inc/tree.h"

int skew;

int main(int argc, char const *argv[])
{    
    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);

    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);

    char *parentFifo = (char*)malloc(50*sizeof(char));
    strcpy(parentFifo, argv[6]);

    int skew = atoi(argv[7]);
    int numOfRecords = atoi(argv[8]);
    int maxHeight = atoi(argv[9]);

    int midRead;
    
    printf("In splitter-merger with %d pid AND SKEW = %d\n", getpid(), skew);
    
    //TODO: Skew part
    if(skew == 0)
        //If skew is 0, uniformly distribute ranges
        midRead = (startRead + endRead)/2;
    else if(skew == 1){
        int sum = 0;        
        for(int i = 0; i < pow(2,maxHeight); i++){
            sum += i;
        }
        //int midRead = (numOfRecords * )/sum;
    }

    char *pidStr = (char*)malloc(20*sizeof(char));
    int pid = getpid();
    sprintf(pidStr,"%d",pid);

    //Make fifo for 1st child - either leafnode or other splitter-mergers
    char *myfifo1 = (char*)malloc(50*sizeof(char));
    strcat(myfifo1, "tmp/fifo1");
    strcat(myfifo1, pidStr);
    if(mkfifo(myfifo1, 0666) < 0 ){
        perror("fifo failed!");
    } 

    //Make fifo for 2nd child - either leafnode or other splitter-mergers
    char *myfifo2 = (char*)malloc(25*sizeof(char));
    strcat(myfifo2, "tmp/fifo2");
    strcat(myfifo2, pidStr);
    if(mkfifo(myfifo2, 0666) < 0 ){
        perror("fifo failed!");
    } 
    
    int fd1, fd2;

    //Create argument array for subsequent processes
    char **argumentArray1 = (char**)malloc(11*sizeof(char*));
	for(int i = 0; i < 11; i++)
		argumentArray1[i] = (char*)malloc(100*sizeof(char));
	
    //Argument array for process 1
    argumentArray1[0] = "exe/splitter-merger";
	sprintf(argumentArray1[1], "%d", height-1);
	sprintf(argumentArray1[2], "%d", startRead);
	sprintf(argumentArray1[3], "%d", midRead);
	argumentArray1[4] = fileName;
	argumentArray1[5] = patternName;
    argumentArray1[6] = myfifo1;
	sprintf(argumentArray1[7], "%d", skew);
    sprintf(argumentArray1[8], "%d", maxHeight);
    sprintf(argumentArray1[9], "%d", numOfRecords);
    argumentArray1[10] = NULL;

    char **argumentArray2 = (char**)malloc(11*sizeof(char*));
	for(int i = 0; i < 11; i++)
		argumentArray2[i] = (char*)malloc(100*sizeof(char));
	
    //Argument array for process 2
    argumentArray2[0] = "exe/splitter-merger";
	sprintf(argumentArray2[1], "%d", height-1);
	sprintf(argumentArray2[2], "%d", midRead);
	sprintf(argumentArray2[3], "%d", endRead);
	argumentArray2[4] = fileName;
	argumentArray2[5] = patternName;
    argumentArray2[6] = myfifo2;
	sprintf(argumentArray2[7], "%d", skew);
    sprintf(argumentArray2[8], "%d", maxHeight);
    sprintf(argumentArray2[9], "%d", numOfRecords);
    argumentArray2[10] = NULL;
    
    int childFd1,childFd2;
    int parentFd;

    //Height > 1 means that child processes are splitter - merger nodes.
    if(height > 1){
        pid_t pid1;
        pid1 = fork();
        //Fork for 1st child
        if (pid1 == 0)
            execvp("exe/splitter-merger", argumentArray1);
        else if(pid1 < 0)
            perror("Error in forking");
        else{
            pid_t pid2;
            pid2 = fork();
            //Fork for 2nd child
            if (pid2 == 0)
            execvp("exe/splitter-merger", argumentArray2);
            else if(pid2 < 0)
                perror("Error in forking");
            else{
                //In parent
                //Open child fifo to read from the child #2 splitter - merger                
                childFd2 = open(myfifo2, O_RDONLY);
                char *tempParent2 = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
                int tmpParentRdFd2;
                fd2 = open(myfifo2, O_RDONLY);
                while(tmpParentRdFd2 != 0){
                    tmpParentRdFd2 = read(childFd2, tempParent2, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                    if( tmpParentRdFd2 < 0 ){
                        perror("Read Fail 2 Parent:");
                    }
                    else{
                        printf("temp2 parent %s\n",tempParent2);
                    }
                }

            }
            //Open child fifo to read from the child #1 splitter - merger  
            childFd1 = open(myfifo1, O_RDONLY);
            char *tempParent1 = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
            int tmpParentRdFd1;
            fd2 = open(myfifo1, O_RDONLY);
            while(tmpParentRdFd1 != 0){
                tmpParentRdFd1 = read(childFd1, tempParent1, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                if( tmpParentRdFd1 < 0 ){
                    perror("Read Fail 1 Parent:");
                }
                else{
                    printf("%s\n",tempParent1);
                }
            }
        }
    }
    //Height == 1 means that children processes are leafNodes
    else if(height == 1){
        pid_t pid1;        
        pid1 = fork();
        if (pid1 == 0){            
            execvp("exe/leafNode", argumentArray1);
        }
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
                double doubleTime2;
                char *temp2 = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
                int tmprdfd2;
                fd2 = open(myfifo2, O_RDONLY);
                parentFd = open(parentFifo, O_WRONLY);
                while(tmprdfd2 != 0){
                    tmprdfd2 = read(fd2, temp2, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                    if( tmprdfd2 < 0 ){
                        perror("Read Fail 2:");
                    }
                    if(temp2[0]=='T'){
                        //write(parentFd, temp2, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                        //WRITE THIS PART TO ROOTNODE
                        temp2++;
                        sscanf(temp2, "%lf\n", &doubleTime2);
                        printf("%lf\n",doubleTime2);
                    }
                    else{
                        write(parentFd, temp2, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                        printf("Temp2 %s\n",temp2);
                    }
                }
                close(fd2);
                close(parentFd);
            }
            double doubleTime1;
            char *temp1 = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
            int tmprdfd1;
            fd1 = open(myfifo1, O_RDONLY);
            parentFd = open(parentFifo, O_WRONLY);
            while(tmprdfd1 != 0){
                tmprdfd1 = read(fd1, temp1, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                if( tmprdfd1 < 0 ){
                    perror("Read Fail 1:");
                }
                if(temp1[0]=='T'){
                    //write(parentFd, temp1, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                    //WRITE THIS PART TO ROOTNODE
                    temp1++;
                    sscanf(temp1, "%lf\n", &doubleTime1);
                    printf("%lf\n",doubleTime1);
                }
                else{
                    write(parentFd, temp1, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
                    printf("Temp1 %s\n",temp1);
                }
            }
            close(fd1);
            close(parentFd);
        }
    }

    return 0;
}