#include "mylib.h"

#include <stdint.h>
#include <windows.h>

int64_t get_pc_frequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart; // ticks per second
}

// milliseconds
int64_t get_ticks() {
	static int64_t pc_freq = get_pc_frequency();
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return ticks.QuadPart * 1000 / pc_freq;
}

int main() {
	int number = 1;
	int64_t time_start = get_ticks();
	while (true) {
		int64_t time_now = get_ticks();
		int64_t elapsed_time = time_now - time_start;

		// if keyboard key pressed
		// print "key pressed"

		if (elapsed_time >= 1000) {
			time_start = time_now;
			hello(number++);
		}
	}
}
