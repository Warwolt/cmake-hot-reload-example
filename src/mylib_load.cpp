#include "mylib.h"

#include <stdio.h>
#include <windows.h>

#ifdef _DEBUG

void hello(int num) {
	HINSTANCE dll_handle = LoadLibrary("build/debug/MyLib.dll");
	if (!dll_handle) {
		printf("Couldn't load DLL!\n");
		return;
	}

	void (*hello_ptr)(int) = (void (*)(int))GetProcAddress(dll_handle, "hello");
	if (!hello_ptr) {
		printf("Couldn't load function in DLL!\n");
		return;
	}

	hello_ptr(num);
}

#endif // _DEBUG
