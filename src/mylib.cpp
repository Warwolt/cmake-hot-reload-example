#include "mylib.h"

#include <stdio.h>

#ifdef _DEBUG
constexpr bool is_shared_library = true;
#else
constexpr bool is_shared_library = false;
#endif

void hello(int num) {
	printf("Hello %s world! %d\n", is_shared_library ? ".dll" : ".lib", num);
}

int next_number(int num) {
	return num + 1;
}
