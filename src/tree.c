#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../inc/tree.h"
#include "../inc/shared.h"

#define CUNIT_TEST 1

FILE *dataFilePtr;

void errCatch(char* errmsg){
	printf("Error: %s\n", errmsg);
}

int strArraySearch(char **array, int len, char *delim){
	for(i = 0; i < len; ++i){
	    if(!strcmp(array[i], delim))
	        return i;
	}
	return -1;
}

int constructProcessTree(int curHeight, int maxHeight, int numOfChildren){
	pid_t  pid;
	if(curHeight == maxHeight)
		return 1;
	if(numOfChildren == 2)
		return 1;
    pid = fork();
    if (pid == 0)
        constructProcessTree(curHeight++,maxHeight,1);
    else 
        constructProcessTree(curHeight,maxHeight,2);
}

int main(int argc, char const *argv[]){

	//UNIT TESTING START

	//If user wants to run unit tests, change define to 1. Otherwise, change to 0.
	//Recommend to run only once then change define to 0
	if(CUNIT_TEST){

		//First CUnit function call; Do not call any other CUnit functions before this
		if(CU_initialize_registry() == CUE_NOMEMORY){
			printf("Could not allocate memory for the testing suite, exiting...\n");
			exit(0);
		}

		CU_pSuite unitCSuite;
		unitCSuite = CU_add_suite("unitCoverageSuite", NULL, NULL);
		if( CU_get_error() != CUE_SUCCESS ){
			printf("Could not add suite, exiting...\n");
			exit(0);
		}

		//Adds the tests declared in tests/tests.c
		CU_pTest test_strArraySearchPtr = CU_add_test(unitCSuite, "test_strArraySearch", test_strArraySearch);

		//Runs all tests in all suites; Outputs to stdout
		CU_basic_run_tests();

		//Final CUnit function call; Do not call any other CUnit functions after this with the 
		//exception of CU_initialize_registry and only to repeat the testing process
		CU_cleanup_registry();
	}

	//UNIT TESTING END

	int count;
	int height;
	char *substr;

	if(argc == 8){
		count = strArraySearch(argv, argc, "-h");
		height = *argv[++count];

		count = strArraySearch(argv, argc, "-d");
		dataFilePtr = fopen(*argv[++count],"r");

		count = strArraySearch(argv, argc, "-p");
		strcpy( substr, *argv[++count] );
	}
	else if(argc == 1){
		height = 3;
		dataFilePtr = stdin;
		//CHANGE SUBSTR ACCORDING TO BINARY FILE
		substr = "test";
	}
	else{
		errCatch("Wrong number of arguments... Exiting\n");
	}

	constructProcessTree(0, height, 1);

}