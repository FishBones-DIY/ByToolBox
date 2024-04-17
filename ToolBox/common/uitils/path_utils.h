#pragma once

#include <shlwapi.h>

#include <string>

namespace path_utils
{
	std::string getUnduplicatedFolderName(const std::string& current_name);
	bool isUnduplicatedFolderName(const std::string& default_name,const std::string& file_name);
	bool isNumber(const std::string& str);

	// ÌìÇæÏà¹Ø
	std::wstring GetTQRegeditPath();

}; // namespace reg_utils

