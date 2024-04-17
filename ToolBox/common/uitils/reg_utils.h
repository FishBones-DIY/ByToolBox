#pragma once

#include <shlwapi.h>

#include <string>

namespace reg_utils
{
	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, DWORD type, void* pv_data, DWORD &size);
	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, std::wstring& data);
	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, int32_t& data);
	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, int64_t& data);
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, DWORD type, LPCVOID pv_data, DWORD size);
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const std::wstring& data);
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const int32_t& data);
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const int64_t& data);
}; // namespace reg_utils

