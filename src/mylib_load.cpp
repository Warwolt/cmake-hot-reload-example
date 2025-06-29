#include <mylib_load.h>
#include <mylib.h>

#include <stdio.h>
#include <windows.h>

#ifdef _DEBUG

struct Library {
	void (*hello)(int num);
};

static Library g_library;

void load_mylib() {
	// FIXME: we should copy the DLL first before loading it to permit hot reloading

	const char* dll_path = "build/debug/MyLib.dll";
	HINSTANCE dll_handle = LoadLibrary(dll_path);
	if (!dll_handle) {
		printf("Couldn't load DLL!\n");
		exit(1);
	}

	void (*fn)(int) = (void (*)(int))GetProcAddress(dll_handle, "hello");
	if (!fn) {
		printf("Couldn't load function %s from %s!\n", "hello", dll_path);
		return;
	}
	g_library.hello = fn;
}

void hello(int num) {
	g_library.hello(num);
}

#endif // _DEBUG
