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
    char *sortArgument = (char*)malloc(1000*sizeof(char));
    strcpy(sortArgument, argv[1]);
    execlp("sort", "-n","-k", "1", "-u", "-o", sortArgument, sortArgument, (char*)NULL);
    return 0;
}
