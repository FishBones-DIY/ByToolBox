#pragma once

#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace file_utils
{
	bool isFileExists(const std::filesystem::path& file_path);

	bool createFile(const std::filesystem::path& file_path);
	bool createDir(const std::filesystem::path& dir_path);

	bool writeFile(const std::filesystem::path& file_path, const std::string& content, std::ios_base::openmode ios_open_mode = std::ios::ate);
	bool writeFile(const std::filesystem::path& file_path, const std::wstring& content, std::ios_base::openmode ios_open_mode = std::ios::ate);

	bool deleteOneFile(const std::filesystem::path& file_path);
	bool deleteDir(const std::filesystem::path& dir_path);

	bool copyOneFile(const std::filesystem::path& source_file_path, const std::filesystem::path& dest_file_path);
	bool copyDir(const std::filesystem::path& source_file_path, const std::filesystem::path& dest_file_path);

	bool moveOneFile(const std::filesystem::path& source_file_path, const std::filesystem::path& dest_file_path);
	bool moveDir(const std::filesystem::path& source_dir_path, const std::filesystem::path& dest_dir_path);
};

