#include "mylib.h"

#include <stdio.h>
#include <windows.h>

#ifdef _DEBUG

void hello() {
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
}

#endif // _DEBUG
