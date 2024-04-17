#include "path_utils.h"

#include <filesystem>

#include "../public/log.h"

#pragma comment(lib, "Shlwapi.lib")

namespace path_utils
{
	std::string getUnduplicatedFolderName(const std::string& current_name)
	{
		std::error_code ec;
		std::filesystem::exists(current_name, ec);
		auto a = ec.message();
		if (std::filesystem::exists(current_name, ec))
		{
			std::filesystem::path p(current_name);
			auto path = p.parent_path().string();
			auto name = p.stem().string();
			auto extension = p.extension().string();
			auto ite_l = name.find_last_of("(");
			if (ite_l == name.npos)
			{
				return getUnduplicatedFolderName(path+"\\"+ name + "(1)"+ extension);
			}
			auto ite_r = name.find_last_of(")");

			std::string sub_string = name.substr(ite_l + 1, ite_r - ite_l - 1);

			if (!isNumber(sub_string))
			{
				return getUnduplicatedFolderName(path + "\\" + name + "(1)" + extension);
			}
			else
			{
				int num = std::stoi(sub_string);

				std::string time = name.substr(0, ite_l);

				p.replace_filename(path + "\\"+time + "(" + std::to_string(++num) + ")" + extension);

				return getUnduplicatedFolderName(p.string());
			}
		}

		return current_name;
	}

	bool isUnduplicatedFolderName(const std::string& default_name, const std::string& file_name)
	{
		if (default_name == file_name)
			return true;

		std::string find_str = default_name + "(";
		auto ite_l = file_name.find(find_str);

		if (ite_l == file_name.npos)
			return false;

		ite_l += find_str.size()-1;

		auto ite_r = file_name.find_last_of(")");

		std::string sub_string = file_name.substr(ite_l + 1, ite_r - ite_l - 1);

		if (!isNumber(sub_string))
			return false;
		else
			return true;
	}

	bool isNumber(const std::string& str)
	{
		bool res = true;
		for (const auto& c : str)
		{
			if (isdigit(c) == 0)//²»ÊÇÊý×Ö
			{
				res = false;
				break;
			}
		}
		return res;
	}

	std::wstring GetTQRegeditPath()
	{
		static std::wstring strRetPath;
		if (!strRetPath.empty())
			return strRetPath;

		std::wstring strProductPath, strDllPath, strTrayPath;
		{
			WCHAR m_wszPath[1024] = { 0 };
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(m_wszPath) - 2;
			if (!(ERROR_SUCCESS == SHGetValue(HKEY_LOCAL_MACHINE
				, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\TQTray.exe"
				, L"Path", &dwType, m_wszPath, &dwSize) && dwType == REG_SZ && dwSize > 0 
				&& PathIsDirectory(m_wszPath)))
			{
				return L"";
			}

			std::error_code ec;
			if (std::filesystem::is_directory(m_wszPath, ec) == false)
				return L"";

			WCHAR wszTrayPath[1024] = { 0 };
			PathCombineW(wszTrayPath, m_wszPath, L"TQTray.exe");

			strProductPath = m_wszPath;
			strTrayPath = wszTrayPath;
			if (!std::filesystem::exists(strTrayPath,ec))
				return L"";
		}

		strRetPath = strProductPath;
		return strRetPath;
	}

}; // namespace path_utils