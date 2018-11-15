#include "../inc/tests.h"
#include <CUnit/CUnit.h>

void test_strArraySearch(){
	const char *arr[3] = {"foo", "bar", "main"};
	CU_ASSERT( strArraySearch(arr, 3, "foo") == 0 );
	CU_ASSERT( strArraySearch(arr, 3, "bar") == 1 );
	CU_ASSERT( strArraySearch(arr, 3, "main") == 2 );
	CU_ASSERT( strArraySearch(arr, 3, "bla") == -1 );
}