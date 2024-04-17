#include "reg_utils.h"

#include "../public/log.h"


#pragma comment(lib, "Shlwapi.lib")

namespace reg_utils
{
	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, DWORD type, void* pv_data, DWORD& size)
	{
		DWORD temp_type = type;
		auto res = SHGetValueW(h_key, item_path.c_str(), sub_key.c_str(), &type, pv_data, &size);
		if (ERROR_SUCCESS != res)
		{
			LOG(ERROR) << "SHGetValue() failed, return gle:" << res
				<< ", item:" << item_path << ", key:" << sub_key;
			return false;
		}

		if (temp_type != type)
		{
			LOG(ERROR) << "SHGetValue() failed, type error,input type:" << temp_type
				<< ", expected type:" << type;
			return false;
		}

		return true;
	}

	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, std::wstring& data)
	{
		WCHAR* buffer_ptr = nullptr;
		DWORD size = 0;

		bool res = getRegValue(h_key, item_path, sub_key, REG_SZ, nullptr, size);
		if (!res)
			return false;

		buffer_ptr = new WCHAR[size/2]();

		if (!buffer_ptr)
		{
			LOG(ERROR) << L"getRegValue() failed, memory allocation failed, gle:" << ::GetLastError();
			return false;
		}

		res = getRegValue(h_key, item_path, sub_key, REG_SZ, (void*)buffer_ptr, size);
		if (res)
			data = buffer_ptr;

		delete buffer_ptr;
		buffer_ptr = nullptr;

		return res;
	}

	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, int32_t& data)
	{
		DWORD size = sizeof(int32_t);
		return getRegValue(h_key, item_path, sub_key, REG_DWORD, (void*)&data, size);
	}

	bool getRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, int64_t& data)
	{
		DWORD size = sizeof(int64_t);
		return getRegValue(h_key, item_path, sub_key, REG_QWORD, (void*)&data, size);
	}

	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, DWORD type, LPCVOID pv_data, DWORD size)
	{
		auto res = SHSetValue(h_key, item_path.c_str(), sub_key.c_str(), type, pv_data, size);
		if (ERROR_SUCCESS != res)
		{
			LOG(ERROR) << "SHSetValue() failed, return gle:" << res
				<< ", item:" << item_path << ", key:" << sub_key << ", value" << pv_data;
			return false;
		}

		return true;
	}

	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const std::wstring& data)
	{
		return setRegValue(h_key, item_path.c_str(), sub_key.c_str(), REG_SZ, data.c_str(), (data.length() + 1) * 2);
	}
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const int32_t& data)
	{
		return setRegValue(h_key, item_path.c_str(), sub_key.c_str(), REG_DWORD, &data, sizeof(int32_t));
	}
	bool setRegValue(HKEY h_key, const std::wstring& item_path, const std::wstring& sub_key, const int64_t& data)
	{
		return setRegValue(h_key, item_path.c_str(), sub_key.c_str(), REG_QWORD, &data, sizeof(int64_t));
	}

}; // namespace reg_utils