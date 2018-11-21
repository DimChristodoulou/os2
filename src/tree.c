#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <time.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../inc/tree.h"
#include "../inc/shared.h"
#include "../inc/tests.h"

void errCatch(char* errmsg){
	printf("Error: %s\n", errmsg);
}

//Function that searches a string array for a string, used to search argv[] array.
int strArraySearch(char const *array[], int len, char *delim){
	for(int i = 0; i < len; ++i){
	    if(!strcmp(array[i], delim))
	        return i;
	}
	return -1;
}

int main(int argc, char const *argv[]){	

	int count;
	int height;
	char *pattern = (char*)malloc(100*sizeof(char));
	FILE *dataFilePtr;
	long lSize;
	MyRecord rec;
	int skew;
	int cunit_test = 0;
	
	//Execute with -s or -test
	if(argc == 8){
		count = strArraySearch(argv, argc, "-h");
		height = atoi(argv[++count]);

		count = strArraySearch(argv, argc, "-d");
		dataFilePtr = fopen(argv[++count],"rb");

		count = strArraySearch(argv, argc, "-p");
		strcpy( pattern, argv[++count] );

		count = strArraySearch(argv, argc, "-s");
		if(count != -1)
			skew = 1;
		
		count = strArraySearch(argv, argc, "-test");
		if(count != -1)
			cunit_test = 1;
	}
	//Execute with -s but without -test
	else if(argc == 7){
		count = strArraySearch(argv, argc, "-h");
		height = atoi(argv[++count]);

		count = strArraySearch(argv, argc, "-d");
		dataFilePtr = fopen(argv[++count],"rb");

		count = strArraySearch(argv, argc, "-p");
		strcpy( pattern, argv[++count] );
		
		skew = 0;
	}
	//Execute with -s and -test
	else if(argc == 9){
		count = strArraySearch(argv, argc, "-h");
		height = atoi(argv[++count]);

		count = strArraySearch(argv, argc, "-d");
		dataFilePtr = fopen(argv[++count],"rb");

		count = strArraySearch(argv, argc, "-p");
		strcpy( pattern, argv[++count] );

		count = strArraySearch(argv, argc, "-s");
		if(count != -1)
			skew = 1;
		
		count = strArraySearch(argv, argc, "-test");
		if(count != -1)
			cunit_test = 1;

		skew = 0;
	}
	//Default arguments for when user calls just ./exe/myfind
	else if(argc == 1){
		height = 3;
		dataFilePtr = stdin;
		pattern = "datafiles/Records100.bin";
		skew = 0;
	}
	else{
		errCatch("Wrong number of arguments... Exiting\n");
		exit(0);
	}

	//UNIT TESTING START
	//If user wants to run unit tests, change define to 1. Otherwise, change to 0.
	//Recommend to run only once then change define to 0
	//If you don't have Cunit, run make cunit
	if(cunit_test==1){

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
		//CU_pTest test_getMinOfDoubleArrayPtr = CU_add_test(unitCSuite, "test_getMinOfDoubleArray", test_getMinOfDoubleArray);
		//CU_pTest test_getMaxOfDoubleArrayPtr = CU_add_test(unitCSuite, "test_getMaxOfDoubleArray", test_getMaxOfDoubleArray);
		//CU_pTest test_getAvgOfDoubleArrayPtr = CU_add_test(unitCSuite, "test_getAvgOfDoubleArray", test_getAvgOfDoubleArray);

		//Runs all tests in all suites; Outputs to stdout
		CU_basic_run_tests();

		//Final CUnit function call; Do not call any other CUnit functions after this with the 
		//exception of CU_initialize_registry and only to repeat the testing process
		CU_cleanup_registry();
		return 1;
	}
	//UNIT TESTING END

	char fileName[100];
	count = strArraySearch(argv, argc, "-d");
	strcpy(fileName, argv[++count]);

	//check number of records
	fseek (dataFilePtr , 0 , SEEK_END);
	lSize = ftell (dataFilePtr);
	rewind (dataFilePtr);
	int numOfrecords = (int) lSize/sizeof(rec);

	fclose(dataFilePtr);

	//Format arguments to pass in root Node executable
	char **argumentArray = (char**)malloc(10*sizeof(char*));
	for(int i = 0; i < 10; i++)
		argumentArray[i] = (char*)malloc(100*sizeof(char));
	
	argumentArray[0] = "exe/rootNode";
	sprintf(argumentArray[1], "%d", height);
	sprintf(argumentArray[2], "%d", 0);
	sprintf(argumentArray[3], "%d", numOfrecords);
	argumentArray[4] = fileName;
	argumentArray[5] = pattern;
	sprintf(argumentArray[6],"%d", skew);
	sprintf(argumentArray[7], "%d", numOfrecords);
	argumentArray[8] = NULL;

	execvp("exe/rootNode",argumentArray);

}