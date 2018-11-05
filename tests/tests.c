#include "tree.c"
#include <CUnit/CUnit.h>

void test_strArraySearch(){
	CU_ASSERT( strArraySearch(["foo", "bar", "main"], 3, "foo") == 0 );
	CU_ASSERT( strArraySearch(["foo", "bar", "main"], 3, "bar") == 1 );
	CU_ASSERT( strArraySearch(["foo", "bar", "main"], 3, "main") == 2 );
	CU_ASSERT( strArraySearch(["foo", "bar", "main"], 3, "bla") == -1 );
}