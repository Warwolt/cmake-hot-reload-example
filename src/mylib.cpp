#ifndef IMPORT_LIBRARY
#define EXPORT_MYLIB
#endif
#include "mylib.h"

#include <stdio.h>
#include <windows.h>

#ifdef SHARED_LIBRARY
constexpr bool is_shared_library = true;
#else
constexpr bool is_shared_library = false;
#endif


// fixme: obviously this needs to go into a separate cpp file
#ifndef IMPORT_LIBRARY
void hello_impl() {
    printf("Hello %s world!\n", is_shared_library ? ".dll" : ".lib");
}
#endif

void hello() {
#ifdef IMPORT_LIBRARY
  HINSTANCE dll_handle = LoadLibrary("build/debug/MyLib.dll");
  if (!dll_handle) {
    printf("Couldn't load DLL!\n");
    return;
  }

    void (*hello_ptr)() = (void(*)())GetProcAddress(dll_handle, "hello");
    if (!hello_ptr) {
        printf("Couldn't load function in DLL!\n");
        return;
    }

    hello_ptr();

#else
    hello_impl();
#endif
}
