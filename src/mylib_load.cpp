#include <mylib.h>
#include <mylib_load.h>

#include <windows.h>

#include <filesystem>
#include <stdio.h>
#include <string>

struct Library {
	void (*hello)(int num);
};

static Library g_library;

void load_mylib() {
#ifdef _DEBUG
	std::string dll_path = "build/debug/MyLib.dll";
	std::string dll_copy_path = "build/debug/MyLib-copy.dll";

	// copy DLL
	std::filesystem::copy(dll_path, dll_copy_path, std::filesystem::copy_options::update_existing);

	// load copy
	HINSTANCE dll_handle = LoadLibrary(dll_copy_path.c_str());
	if (!dll_handle) {
		printf("Couldn't load DLL!\n");
		exit(1);
	}

	void (*fn)(int) = (void (*)(int))GetProcAddress(dll_handle, "hello");
	if (!fn) {
		printf("Couldn't load function %s from %s!\n", "hello", dll_path.c_str());
		return;
	}
	g_library.hello = fn;
#endif // _DEBUG
}

#ifdef _DEBUG
void hello(int num) {
	g_library.hello(num);
}
#endif // _DEBUG
