#include <mylib.h>
#include <mylib_load.h>

#include <windows.h>

#include <filesystem>
#include <stdio.h>
#include <string>

struct Library {
	HINSTANCE handle;
	void (*hello)(int num);
};

static Library g_library;

void load_mylib() {
#ifdef _DEBUG
	std::string dll_path = "build/debug/MyLib.dll";
	std::string dll_copy_path = "build/debug/MyLib-copy.dll";

	// unload DLL
	if (g_library.handle) {
		FreeLibrary(g_library.handle);
	}

	// copy DLL
	std::filesystem::copy(dll_path, dll_copy_path, std::filesystem::copy_options::update_existing);

	// load copy
	g_library.handle = LoadLibrary(dll_copy_path.c_str());
	if (!g_library.handle) {
		printf("Couldn't load DLL!\n");
		return;
	}

	// FIXME: generalize this so we can easily express loading multiple different functions
	void (*fn)(int) = (void (*)(int))GetProcAddress(g_library.handle, "hello");
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
