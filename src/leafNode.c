#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <time.h>
#include <math.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/shared.h"
#include "../inc/tree.h"

#define DELIMITER " | "

int main(int argc, char *argv[]){ 
    clock_t begin = clock();
    int height = atoi(argv[1]);
    int startRead = atoi(argv[2]);
    int endRead = atoi(argv[3]);
    char* fileName = (char*)malloc(100*sizeof(char));
    char* patternName = (char*)malloc(100*sizeof(char));
    char* myfifo = (char*)malloc(25*sizeof(char));
    char* ptr;
    strcpy(fileName, argv[4]);
    strcpy(patternName, argv[5]);
    strcpy(myfifo, argv[6]);
    int skew = atoi(argv[7]);
    int maxHeight = atoi(argv[9]);
    int numOfRecords = atoi(argv[8]);
    int rootPID = atoi(argv[10]);
    int fd;    
    
    //If skew is selected, calculate two sums, the sum of all ids of the children (so for 4 children 1+2+3+4) and one from 0 to the 
    //current child's id (so for 4 children, if current leafnode is child #3, this sum will be 1+2)
    //Afterwards, calculate the new starting and ending point.
    if(skew == 1){        
        int sum = 0, sum2 = 0;          
        for(int i = 1; i < pow(2, maxHeight) + 1; i++){
            sum += i;
        }        
        for(int j = 0; j < startRead; j++)
        {
            sum2 += j;
        }        
        startRead = (numOfRecords*sum2)/sum;
        endRead = (numOfRecords*endRead)/sum;
        endRead += startRead;
    }

    printf("In\t leafNode\t with\t %d pid, %s myfifo\n", getpid(), myfifo);

    MyRecord searchRec;

    char *searchStr = (char*)malloc( (5*SIZEofBUFF + SizeofFLOAT + SizeofINT + SizeofLONG + 10) * sizeof(char));
    
    char *custIdStr = (char*)malloc(20*sizeof(char));
    char *houseIdStr = (char*)malloc(20*sizeof(char));
    char *amountStr = (char*)malloc(20*sizeof(char));
    
    FILE *fptr = fopen(fileName, "rb");
    fseek( fptr, startRead*sizeof(MyRecord), SEEK_SET );

    fd = open(myfifo, O_WRONLY);

    for(int i = startRead; i < endRead; i++){
        fread(&searchRec, sizeof(MyRecord), 1, fptr);  
        //Empty string every time we read a record
        searchStr[0] = '\0'; 
        
        sprintf(custIdStr, "%ld", searchRec.custid);
        sprintf(houseIdStr, "%d", searchRec.HouseID);
        sprintf(amountStr, "%-9.2f", searchRec.amount);

        if( strstr(custIdStr, patternName)!=NULL || strstr(searchRec.LastName, patternName)!=NULL || 
        strstr(searchRec.FirstName, patternName)!=NULL || strstr(searchRec.Street, patternName)!=NULL ||
        strstr(houseIdStr, patternName)!=NULL || strstr(searchRec.City, patternName)!=NULL || strstr(searchRec.postcode, patternName)!=NULL || 
        strstr(amountStr, patternName)!=NULL){

            //Build a string containing the whole record and write it to the parent
            searchStr = strcat(searchStr, custIdStr);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, searchRec.LastName);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, searchRec.FirstName);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, searchRec.Street);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, houseIdStr);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, searchRec.City);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, searchRec.postcode);
            searchStr = strcat(searchStr, DELIMITER);
            searchStr = strcat(searchStr, amountStr);
            write(fd, searchStr, (5*SIZEofBUFF + SizeofFLOAT + SizeofINT + SizeofLONG + 50));

        }
    }
    
    //Stop counting time, process is finished, write time to parent process and send SIGUSR2 to root node
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    char output[50];
    snprintf(output, 50, "T%f", time_spent);
    write(fd, output, 50);
    close(fd);
    kill(rootPID, SIGUSR2);
    //fclose(fptr);
    //free(fileName);
    //free(patternName);
    free(myfifo);
    free(ptr);
    free(searchStr);
    free(custIdStr);
    free(houseIdStr);
    free(amountStr);
}