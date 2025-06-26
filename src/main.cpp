#include "mylib.h"

#include <windows.h>

int main() {
	int number = 1;
	while (true) {
		hello(number++);
		Sleep(1000);
	}
}
