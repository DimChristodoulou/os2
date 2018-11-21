#include "../inc/tests.h"
#include <CUnit/CUnit.h>

void test_strArraySearch(){
	const char *arr[3] = {"foo", "bar", "main"};
	CU_ASSERT( strArraySearch(arr, 3, "foo") == 0 );
	CU_ASSERT( strArraySearch(arr, 3, "bar") == 1 );
	CU_ASSERT( strArraySearch(arr, 3, "main") == 2 );
	CU_ASSERT( strArraySearch(arr, 3, "bla") == -1 );
}

// void test_getMinOfDoubleArray(){
// 	double arr[5] = {1.5, 1, 2.7, 4.5, 6.3};
// 	CU_ASSERT( getMinOfDoubleArray(arr, 5) == 1 );
// 	double barr[5] = {1.5, 1.1, 2.7, 4.5, 6.3};
// 	CU_ASSERT( getMinOfDoubleArray(barr, 5) == 1.1 );
// 	double carr[5] = {0, 0, 0, 0, 0};
// 	CU_ASSERT( getMinOfDoubleArray(carr, 5) == 0 );
// }

// void test_getMaxOfDoubleArray(){
// 	double arr[5] = {1.5, 1, 2.7, 4.5, 6.3};
// 	CU_ASSERT( getMaxOfDoubleArray(arr, 5) == 6.3 );
// 	double barr[5] = {1.5, 7, 2.7, 4.5, 6.3};
// 	CU_ASSERT( getMaxOfDoubleArray(barr, 5) == 7 );
// 	double carr[5] = {0, 0, 0, 0, 0};
// 	CU_ASSERT( getMaxOfDoubleArray(carr, 5) == 0 );
// }

// void test_getAvgOfDoubleArray(){
// 	double arr[5] = {1.5, 1, 2.7, 4.5, 6.3};
// 	CU_ASSERT( getAvgOfDoubleArray(arr, 5) == 3.2 );
// 	double barr[5] = {0, 0, 0, 0, 0};
// 	CU_ASSERT( getAvgOfDoubleArray(barr, 5) == 0 );
//}