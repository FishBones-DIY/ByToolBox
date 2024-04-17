#pragma once

#include <string>

namespace string_utils
{
	std::string trim(const std::string& s, char c = ' ');

	bool startsWithW(std::wstring s, std::wstring sub);
}; // namespace reg_utils

