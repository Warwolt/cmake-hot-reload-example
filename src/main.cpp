#include <mylib.h>
#include <mylib_load.h>
#include <run_command.h>

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

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

void reload_code() {
#ifdef _DEBUG
	bool build_ok = run_cmake_build();
	if (build_ok) {
		printf("trying to reload lib\n");
		load_mylib();
	}
#endif
}

void update_hot_reloading() {
#ifdef _DEBUG
	static bool prev_button_state = false;
	bool button_state = GetKeyState(VK_F10) & 0x8000;
	bool button_pressed_now = button_state && !prev_button_state;
	prev_button_state = button_state;
	if (button_pressed_now) {
		reload_code();
	}
#endif
}

void initialize() {
#ifdef _DEBUG
	load_mylib();
#endif
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
			hello(number++);
		}
	}
}
