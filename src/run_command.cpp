#include <run_command.h>

#include <windows.h>

std::string get_win32_error() {
	DWORD err_code = GetLastError();
	char* err_msg;
	if (!FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
			(LPTSTR)&err_msg,
			0,
			NULL
		)) {
		return "Unknown error";
	}

	static char buffer[1024];
	_snprintf_s(buffer, sizeof(buffer), "%s", err_msg);
	LocalFree(err_msg);

	return std::string(buffer);
}

std::expected<ExitCode, std::string> run_command(std::string cmd) {
	// this function based on:
	// https://learn.microsoft.com/en-gb/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN

	/* Set up pipes */
	HANDLE stdout_read;
	HANDLE stdout_write;
	HANDLE stderr_read;
	HANDLE stderr_write;
	{
		SECURITY_ATTRIBUTES security_attr = {};
		security_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
		security_attr.bInheritHandle = TRUE;
		security_attr.lpSecurityDescriptor = NULL;

		if (!CreatePipe(&stdout_read, &stdout_write, &security_attr, 0)) {
			return std::unexpected("stdout CreatePipe failed: " + get_win32_error());
		}

		if (!CreatePipe(&stderr_read, &stderr_write, &security_attr, 0)) {
			return std::unexpected("stderr CreatePipe failed: " + get_win32_error());
		}

		if (!SetHandleInformation(stdout_read, HANDLE_FLAG_INHERIT, 0)) {
			return std::unexpected("stdout SetHandleInformation failed: " + get_win32_error());
		}

		if (!SetHandleInformation(stderr_read, HANDLE_FLAG_INHERIT, 0)) {
			return std::unexpected("stdout SetHandleInformation failed: " + get_win32_error());
		}
	}

	/* Run command */
	PROCESS_INFORMATION process_info = {};
	{
		STARTUPINFO startup_info = {};
		startup_info.cb = sizeof(STARTUPINFO);
		startup_info.hStdOutput = stdout_write;
		startup_info.hStdError = stderr_write;
		startup_info.dwFlags = STARTF_USESTDHANDLES;

		bool did_create_process = CreateProcess(
			NULL, // path
			&cmd[0],
			NULL, // process security attributes
			NULL, // primary thread security attributes
			TRUE, // handles are inherited
			0,    // creation flags
			NULL, // use parent's environment
			NULL, // use parent's current directory
			&startup_info,
			&process_info
		);

		// close handles
		CloseHandle(stdout_write);
		CloseHandle(stderr_write);

		if (!did_create_process) {
			return std::unexpected("CreateProcess(\"" + cmd + "\") failed: " + get_win32_error());
		}
	}

	/* Read from stdout until done */
	// Read stdout and stderr live (alternating reads)
	char buffer[256];
	DWORD bytes_read;
	DWORD available;
	bool process_running = true;

	while (process_running || PeekNamedPipe(stdout_read, NULL, 0, NULL, &available, NULL) && available > 0 ||
		   PeekNamedPipe(stderr_read, NULL, 0, NULL, &available, NULL) && available > 0) {
		if (PeekNamedPipe(stdout_read, NULL, 0, NULL, &available, NULL) && available > 0) {
			if (ReadFile(stdout_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) {
				buffer[bytes_read] = '\0';
				fprintf(stdout, "%s", buffer);
				fflush(stdout);
			}
		}

		if (PeekNamedPipe(stderr_read, NULL, 0, NULL, &available, NULL) && available > 0) {
			if (ReadFile(stderr_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) {
				buffer[bytes_read] = '\0';
				fprintf(stderr, "%s", buffer);
				fflush(stderr);
			}
		}

		// Check if process is still running
		DWORD wait_result = WaitForSingleObject(process_info.hProcess, 50);
		process_running = (wait_result == WAIT_TIMEOUT);
	}

    // close read pipes
	CloseHandle(stdout_read);
	CloseHandle(stderr_read);

	/* Read exit code */
	DWORD exit_code;
	if (!GetExitCodeProcess(process_info.hProcess, &exit_code)) {
		return std::unexpected("GetExitCodeProcess failed: " + get_win32_error());
	}

	/* Finish */
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);

	return (int)exit_code;
}
