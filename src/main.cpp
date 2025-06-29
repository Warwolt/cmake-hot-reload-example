#include <mylib.h>
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

void run_cmake_build() {
	std::expected<ExitCode, std::string> result = run_command("cmake --build build --config Debug");
	if (!result) {
		printf("Error: %s", result.error().c_str());
	}
}

int main() {
	int number = 1;
	int64_t time_start = time_now_ms();
	bool prev_button_state = false;
	while (true) {
		int64_t time_now = time_now_ms();
		int64_t elapsed_time = time_now - time_start;

		bool button_state = GetKeyState(VK_F10) & 0x8000;
		bool button_pressed_now = button_state && !prev_button_state;
		prev_button_state = button_state;
		if (button_pressed_now) {
			run_cmake_build();
		}

		if (elapsed_time >= 1000) {
			time_start = time_now;
			hello(number++);
		}
	}
}
