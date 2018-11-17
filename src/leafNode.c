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

int main(int argc, char *argv[]){ 

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
    int fd;
    printf("In leafNode with %d pid, %d height, %d startRead, %d endRead, %s fileName, %s patternName %s myfifo\n", getpid(), height, startRead, endRead, fileName, patternName, myfifo);

    MyRecord searchRec;
    //char *searchStr = (char*)malloc( (5*SIZEofBUFF + SizeofFLOAT + SizeofINT + SizeofLONG) * sizeof(char));
    char *custIdStr = (char*)malloc(20*sizeof(char));
    char *houseIdStr = (char*)malloc(20*sizeof(char));
    char *amountStr = (char*)malloc(20*sizeof(char));

    fd = open(myfifo, O_WRONLY);

    FILE *fptr = fopen(fileName, "rb");
    fseek( fptr, startRead*sizeof(MyRecord), SEEK_SET );
    for(int i = startRead; i < endRead; i++){
        fread(&searchRec, sizeof(MyRecord), 1, fptr);   
        
        sprintf(custIdStr, "%ld", searchRec.custid);
        sprintf(houseIdStr, "%d", searchRec.HouseID);
        sprintf(amountStr, "%-9.2f", searchRec.amount);

        if( strstr(custIdStr, patternName)!=NULL || strstr(searchRec.LastName, patternName)!=NULL || 
        strstr(searchRec.FirstName, patternName)!=NULL || strstr(searchRec.Street, patternName)!=NULL ||
        strstr(houseIdStr, patternName)!=NULL || strstr(searchRec.City, patternName)!=NULL || strstr(searchRec.postcode, patternName)!=NULL || 
        strstr(amountStr, patternName)!=NULL){

            
            write(fd, &searchRec, sizeof(MyRecord)+1);
            //printf("%ld %s %s  %s %d %s %s %-9.2f\n", searchRec.custid, searchRec.LastName, searchRec.FirstName, searchRec.Street, searchRec.HouseID, searchRec.City, searchRec.postcode, searchRec.amount);
        }
    }
    write(fd, "test", 6);
    printf("hello\n");
    close(fd);
    printf("bye\n");
}