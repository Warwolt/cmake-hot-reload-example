#pragma once

#include <expected>
#include <optional>
#include <string>

#include <windows.h>

using ExitCode = int;
std::expected<ExitCode, std::string> run_command(std::string cmd);
std::optional<FILETIME> file_last_modified(std::string file_path);
