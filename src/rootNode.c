#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <math.h>
#include <signal.h> 
#include <time.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/tree.h"
#include "../inc/rootNode.h"
#include "../inc/shared.h"

int sigCount;

void user2Sig_Handler(){
    sigCount++;
}

double getMinOfDoubleArray(double *doubleArray, int size){
    double min = doubleArray[0];
    for(int i = 1; i < size; i++){
        if(doubleArray[i]<min){
            min = doubleArray[i];
        }
    }
    return min;
}

double getMaxOfDoubleArray(double *doubleArray, int size){
    double max = doubleArray[0];
    for(int i = 1; i < size; i++){
        if(doubleArray[i]>max){
            max = doubleArray[i];
        }
    }
    return max;
}

double getAvgOfDoubleArray(double *doubleArray, int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += doubleArray[i];
    }    
    return sum/(double)size;
}

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    //Assign signal SIGUSR2 to function user2Sig_Handler
    signal(SIGUSR2, user2Sig_Handler);
    sleep(0.5);
    pid_t pid;
    printf("In\t rootNode\t with\t %d pid\n", getpid());

    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);
    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);
    int skew = atoi(argv[6]);
    int numOfRecords = atoi(argv[7]);

    double **timeArray = (double**)malloc(2*sizeof(double*));
    timeArray[0] = (double*)malloc((pow(2,height)+1)*sizeof(double));
    timeArray[1] = (double*)malloc((pow(2,height)+1)*sizeof(double));    

    if(skew == 1){
        startRead = 1;
        endRead = pow(2, height);
    }    

    //Build the array of arguments that gets passed over to the other executables
	char **argumentArray = (char**)malloc(15*sizeof(char*));
	for(int i = 0; i < 15; i++)
		argumentArray[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray[0] = "exe/splitter-merger";
	sprintf(argumentArray[1], "%d", height);
    //If skew is 0, uniformly distribute ranges
	sprintf(argumentArray[2], "%d", startRead);
	sprintf(argumentArray[3], "%d", endRead);
	argumentArray[4] = fileName;
	argumentArray[5] = patternName;
    sprintf(argumentArray[7], "%d", skew);
    sprintf(argumentArray[8], "%d", numOfRecords);
    sprintf(argumentArray[9], "%d", height);

    char *pidStr = (char*)malloc(20*sizeof(char));
    int myPid = getpid();
    sprintf(pidStr,"%d",myPid);

    char *myRootFifo = (char*)malloc(50*sizeof(char));
    strcat(myRootFifo, "tmp/masterFifo");
    strcat(myRootFifo, pidStr);
    if(mkfifo(myRootFifo, 0666) < 0 ){
        perror("fifo failed!");
    } 
    argumentArray[6] = myRootFifo;
    sprintf(argumentArray[10],"%d",getpid());

	argumentArray[11] = NULL;

    int childRootFd;    
    FILE *fp;

    pid = fork();
    if (pid == 0){
        execvp("exe/splitter-merger", argumentArray);
	}
    else if(pid < 0){
        perror("Error in forking");
    }
    else{        
        fp = fopen("output","w");
        char *tempParent = (char*)malloc((5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50)*sizeof(char));
        int retReadRoot = -1;
        int leafNodeCount = 0;
        int parentNodeCount = 0;
        childRootFd = open(myRootFifo, O_RDONLY);
        //While there is still data to read...
        while(retReadRoot != 0){
            retReadRoot = read(childRootFd, tempParent, (5*SIZEofBUFF + SizeofINT + SizeofFLOAT + SizeofLONG + 50));
            //<0 is false read, return error
            if( retReadRoot < 0 ){
                perror("Read Fail Root:");
            }
            //'T' is an identifier for metadata sent by the leaf Nodes, specifically time used by those nodes.
            //Example: T0.00379 shows that a leaf node was being run for 3.79msec
            if(tempParent[0]=='T'){
                tempParent++;
                sscanf(tempParent, "%lf\n", &timeArray[0][leafNodeCount++]);                
            }
            //'T' is an identifier for metadata sent by the Splitter-Merger Nodes, specifically time used by those nodes.
            //Example: L0.00379 shows that a splitter-merger node was being run for 3.79msec
            else if(tempParent[0]=='L'){
                tempParent++;
                sscanf(tempParent, "%lf\n", &timeArray[1][parentNodeCount++]);                
            }
            else{                
                if(tempParent[0]!='0')
                    fprintf(fp, "%s\n", tempParent);
            }
        }
    }    

    printf("\n%d CHILDREN SENT SIGUSR2 SIGNAL\n",sigCount);

    //Stop counting time, query is done
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTIME TO FINISH QUERY: %f\n", time_spent);

    printf("\nMINIMUM LEAF PROCESS TIME IS %f\n", getMinOfDoubleArray(timeArray[0],pow(2,height)));
    printf("MAXIMUM LEAF PROCESS TIME IS %f\n", getMaxOfDoubleArray(timeArray[0],pow(2,height)));
    printf("AVERAGE LEAF PROCESS TIME IS %f\n", getAvgOfDoubleArray(timeArray[0],pow(2,height)));

    printf("\nMINIMUM SPLITTER-MERGER PROCESS TIME IS %f\n", getMinOfDoubleArray(timeArray[1],pow(2,height)));
    printf("MAXIMUM SPLITTER-MERGER PROCESS TIME IS %f\n", getMaxOfDoubleArray(timeArray[1],pow(2,height)));
    printf("AVERAGE SPLITTER-MERGER PROCESS TIME IS %f\n", getAvgOfDoubleArray(timeArray[1],pow(2,height)));       

    // if(fclose(fp)!=0){
    //     perror("FILE CLOSE ERROR");
    // }

	//execlp("exe/sortNode","exe/sortNode", "output",(char*)NULL);

    return 0;
}
