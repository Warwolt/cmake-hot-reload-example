#pragma once

#include <expected>
#include <string>

using ExitCode = int;
std::expected<ExitCode, std::string> run_command(std::string cmd);
