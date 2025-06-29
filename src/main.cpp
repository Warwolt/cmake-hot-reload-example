#include <mylib.h>
#include <mylib_load.h>
#include <win32.h>

#include <windows.h>

#include <stdint.h>
#include <stdio.h>

#ifdef _DEBUG
constexpr bool debug_build = true;
#else
constexpr bool debug_build = false;
#endif

int64_t get_pc_frequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart; // ticks per second
}

int64_t time_now_ms() {
	static int64_t pc_freq = get_pc_frequency();
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return ticks.QuadPart * 1000 / pc_freq;
}

bool run_cmake_build() {
	std::string cmd = "cmake --build build --target MyLibShared --config Debug";
	printf("Running command: %s\n", cmd.c_str());
	std::expected<ExitCode, std::string> result = run_command(cmd);
	if (!result) {
		printf("Error: %s", result.error().c_str());
		return false;
	}
	return true;
}

void update_hot_reloading() {
	if constexpr (debug_build) {
		/* Rebuild library on button press */
		static bool prev_button_state = false;
		bool button_state = GetKeyState(VK_F10) & 0x8000;
		bool button_pressed_now = button_state && !prev_button_state;
		prev_button_state = button_state;
		if (button_pressed_now) {
			run_cmake_build();
		}

		/* Reload library if the file has changed */
		std::string dll_path = "build/debug/MyLib.dll";
		static FILETIME last_time_modified = file_last_modified(dll_path).value();
		if (std::optional<FILETIME> time_modified = file_last_modified(dll_path)) {
			if (CompareFileTime(&time_modified.value(), &last_time_modified) != 0) {
				last_time_modified = *time_modified;
				load_mylib();
			}
		}
	}
}

void initialize() {
	if constexpr (debug_build) {
		load_mylib();
	}
}

int main() {
	initialize();

	int number = 1;
	int64_t time_start = time_now_ms();
	bool prev_button_state = false;
	while (true) {
		int64_t time_now = time_now_ms();
		int64_t elapsed_time = time_now - time_start;

		update_hot_reloading();

		if (elapsed_time >= 1000) {
			time_start = time_now;
			hello(number);
			number = next_number(number);
		}
	}
}
