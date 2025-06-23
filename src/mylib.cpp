#define EXPORT_MYLIB
#include "mylib.h"

#include <stdio.h>

void hello() {
    printf("Hello world! MYLIB_API =%s\n", MYLIB_API_STR);
}
