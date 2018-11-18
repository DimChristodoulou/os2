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
    int skew = atoi(argv[7]);
    printf("In splitter-merger with %d pid AND SKEW = %d\n", getpid(), skew);
    int midRead;

    if(skew == 0)
        midRead = (startRead + endRead)/2;
    else if(skew == 1){
        int sum = 0;
        
        for(int i = 0; i < pow(2,height); i++){
            
        }
        
    }


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

    char **argumentArray1 = (char**)malloc(9*sizeof(char*));
	for(int i = 0; i < 9; i++)
		argumentArray1[i] = (char*)malloc(100*sizeof(char));
	
    argumentArray1[0] = "exe/splitter-merger";
	sprintf(argumentArray1[1], "%d", height-1);
	sprintf(argumentArray1[2], "%d", startRead);
	sprintf(argumentArray1[3], "%d", midRead);
	argumentArray1[4] = fileName;
	argumentArray1[5] = patternName;
    argumentArray1[6] = myfifo1;
	sprintf(argumentArray1[7], "%d", skew);
    argumentArray1[8] = NULL;

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
	sprintf(argumentArray2[7], "%d", skew);
    argumentArray2[8] = NULL;
    
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
                double doubleTime2;
                char *temp2 = (char*)malloc((sizeof(MyRecord) + 55)*sizeof(char));

                fd2 = open(myfifo2, O_RDONLY);
                int tmprdfd2 = read(fd2, temp2, 50);
                if( tmprdfd2 < 0 || tmprdfd2 == EOF){
                    perror("Read Fail 2:");
                }

                if(temp2[0]=='T'){
                    temp2++;
                    sscanf(temp2, "%lf", &doubleTime2);
                    printf("%lf\n",doubleTime2);
                    close(fd2);
                }
                else{
                    //tmprdfd2 = read(fd2, &rec2, (sizeof(MyRecord)));
                    close(fd2);
                    FILE *fp2 = fopen(myfifo2, "rb");
                    fread(&rec2, sizeof(MyRecord), 1, fp2);
                    if( tmprdfd2 < 0 || tmprdfd2 == EOF){
                        perror("Read Fail 2:");
                    }
                    printf("IN 2 %ld %s %s %s %d %s %s %-9.2f\n", rec2.custid, rec2.LastName, rec2.FirstName, rec2.Street, rec2.HouseID, rec2.City, rec2.postcode, rec2.amount);
                    fclose(fp2);
                }
                //printf("IN 2 %ld %s %s %s %d %s %s %-9.2f\n", rec2.custid, rec2.LastName, rec2.FirstName, rec2.Street, rec2.HouseID, rec2.City, rec2.postcode, rec2.amount);
                close(fd2);
            }
            //wait(NULL);
            MyRecord rec1;
            double doubleTime1;
            char *temp1 = (char*)malloc(50*sizeof(char));

            fd1 = open(myfifo1, O_RDONLY);
            int tmprdfd1 = read(fd1, temp1, 50);
            if( tmprdfd1 < 0 || tmprdfd1 == EOF){
                perror("Read Fail 1:");
            }
            if(temp1[0]=='T'){
                temp1++;
                sscanf(temp1, "%lf", &doubleTime1);
                printf("%lf\n",doubleTime1);
                close(fd1);
            }
            else{
                close(fd1);
                FILE *fp1 = fopen(myfifo1, "rb");
                fread(&rec1, sizeof(MyRecord), 1, fp1);
                if( tmprdfd1 < 0 || tmprdfd1 == EOF){
                    perror("Read Fail 1:");
                }
                printf("IN 1 %ld %s %s %s %d %s %s %-9.2f\n", rec1.custid, rec1.LastName, rec1.FirstName, rec1.Street, rec1.HouseID, rec1.City, rec1.postcode, rec1.amount);
                fclose(fp1);
            }
            //printf("IN 1 %ld %s %s %s %d %s %s %-9.2f\n", rec1.custid, rec1.LastName, rec1.FirstName, rec1.Street, rec1.HouseID, rec1.City, rec1.postcode, rec1.amount);            
        }
    }

    return 0;
}