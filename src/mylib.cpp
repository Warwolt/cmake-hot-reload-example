#include "mylib.h"

#include <stdio.h>

#ifdef _DEBUG
constexpr bool is_shared_library = true;
#else
constexpr bool is_shared_library = false;
#endif

void hello() {
    printf("Hello %s world!\n", is_shared_library ? ".dll" : ".lib");
}
