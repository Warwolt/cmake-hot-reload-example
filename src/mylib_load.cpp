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

template <typename FN>
FN load_function(HINSTANCE dll_handle, const char* function_name) {
	FN fn = (FN)GetProcAddress(dll_handle, function_name);
	if (!fn) {
		printf("Couldn't load function %s from DLL!\n", function_name);
		return nullptr;
	}
	return fn;
}

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

	if (auto* fn = load_function<decltype(Library::hello)>(g_library.handle, "hello")) {
		g_library.hello = fn;
	}

	#endif // _DEBUG
}

#ifdef _DEBUG
void hello(int num) {
	g_library.hello(num);
}
#endif // _DEBUG
