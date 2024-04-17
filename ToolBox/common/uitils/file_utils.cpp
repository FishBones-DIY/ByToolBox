#include "common/public/log.h"

#include <fstream>

#include "file_utils.h"

#define Check_Param_Empty(path)        if (path.empty())                                                   \
                                       {                                                                   \
                                           LOG(ERROR) << __func__ << " " << #path <<" parameter is empty"; \
                                           return false;                                                   \
                                       }

#define Check_Exists(path)             if (!fs::exists(path, ec))                                          \
	                                   {                                                                   \
	                                       LOG(ERROR) << __func__ << " file not exists:" << path;          \
	                                       return false;                                                   \
	                                   }																    

#define Check_Is_Dir(path)             if (!fs::is_directory(path, ec))                                    \
	                                   {                                                                   \
	                                   	   LOG(ERROR) << __func__ << " path is not directory:" << path;    \
	                                   	   return false;                                                   \
	                                   }

#define Check_Is_Simple_File(path)     if (!fs::is_regular_file(path, ec))                                 \
	                                   {                                                                   \
	                                   	   LOG(ERROR) << __func__ << " path is not regular_file:" << path; \
	                                   	   return false;                                                   \
	                                   }

#define Get_Param_Name(param)          #param

namespace file_utils
{
	void removeEndChar(fs::path& path, char c)
	{
		while (!path.string().empty())
		{
			if (path.string().back() == c)
			{
				auto path_str = path.string();
				path_str.pop_back();
				path = path_str;
			}
			else
				break;
		}
	}

	void removeEndSlash(fs::path& path)
	{
		removeEndChar(path, '\\');
		removeEndChar(path, '/');
	}

	bool isDirector(const fs::path& path)
	{
		fs::path temp_path = path;

		char end_char = temp_path.string().back();
		std::error_code ec;
		if (fs::is_directory(path, ec) || '/' == end_char || '\\' == end_char)
		{
			return true;
		}
		return false;
	}

	bool isFileExists(const fs::path& file_path)
	{
		Check_Param_Empty(file_path);

		std::error_code ec;
		return fs::exists(file_path, ec);
	}

	bool createFile(const fs::path& file_path)
	{
		Check_Param_Empty(file_path);

		std::error_code ec;

		if (fs::exists(file_path, ec))
			return true;

		std::ofstream file(file_path);
		if (!file.is_open()) {
			ec = std::error_code(errno, std::generic_category());
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", file path:" << file_path;
			return false;
		}
		else {
			file.close();
			return true;
		}
	}

	bool createDir(const fs::path& dir_path)
	{
		Check_Param_Empty(dir_path);

		std::error_code ec;
		fs::create_directories(dir_path, ec);
		if (ec)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", create dir: " << dir_path;
			return false;
		}

		return true;
	}

	template <typename StringType>//只能是std::string或者std::wstring
	using StringOrWString = typename std::enable_if<std::is_same<StringType, std::string>::value || std::is_same<StringType, std::wstring>::value, StringType>::type;
	template <typename StringType>//如果是std::string就用std::ofstream，否则就用std::wofstream
	using Fstream = typename std::conditional<std::is_same<StringType, std::string>::value, std::ofstream, std::wofstream>::type;

	template <typename StringType>
	bool writeFileTemplate(const std::filesystem::path& file_path, const StringOrWString<StringType>& content, std::ios_base::openmode ios_open_mode)
	{
		Check_Param_Empty(file_path);

		if (file_path.is_absolute() && !createDir(file_path.parent_path()))
			return false;

		bool result = false;
		Fstream<StringType> writer;
		writer.imbue(std::locale(""));//设置本地字符集，否则写入中文可能会失败。
		writer.open(file_path, ios_open_mode);
		if (writer.is_open())
		{
			writer.write(content.c_str(), content.size());
			if (!writer.fail())
			{
				result = true;
			}
			writer.close();
		}

		if (!result)
		{
			std::error_code ec = std::error_code(errno, std::generic_category());
			LOG(ERROR) << __func__ << " failed!"
				<< ", ios_open_mode: " << ios_open_mode
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", file path: " << file_path;
		}

		return result;
	}

	bool writeFile(const std::filesystem::path& file_path, const std::string& content, std::ios_base::openmode ios_open_mode /*= std::ios::ate*/)
	{
		return writeFileTemplate<std::string>(file_path, content, ios_open_mode);
	}

	bool writeFile(const std::filesystem::path& file_path, const std::wstring& content, std::ios_base::openmode ios_open_mode /*= std::ios::ate*/)
	{
		return writeFileTemplate<std::wstring>(file_path, content, ios_open_mode);
	}

	bool deleteOneFile(const fs::path& file_path)
	{
		Check_Param_Empty(file_path);

		std::error_code ec;

		Check_Is_Simple_File(file_path);
		
		bool ret = fs::remove(file_path, ec);
		if (!ret)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", file path:" << file_path;
		}
		return ret;
	}

	bool deleteDir(const fs::path& dir_path)
	{
		Check_Param_Empty(dir_path);

		std::error_code ec;
		fs::remove_all(dir_path, ec);
		if (ec)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", file path:" << dir_path;
			return false;
		}

		return true;
	}

	bool copyOneFile(const fs::path& source_file_path, const fs::path& dest_file_path)
	{
		Check_Param_Empty(source_file_path);
		Check_Param_Empty(dest_file_path);

		std::error_code ec;

		Check_Exists(source_file_path);

		if (!createDir(dest_file_path.parent_path()))
			return false;

		fs::path real_dest_file_path = dest_file_path;
		if (isDirector(real_dest_file_path))
		{
			real_dest_file_path = dest_file_path / source_file_path.filename();
		}

		fs::copy_file(source_file_path, real_dest_file_path, fs::copy_options::overwrite_existing, ec);
		if (ec)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", source_file_path:" << source_file_path
				<< ", dest_file_path:" << real_dest_file_path;
			return false;
		}

		return true;
	}

	bool copyDir(const fs::path& source_file_path, const fs::path& dest_file_path)
	{
		Check_Param_Empty(source_file_path);
		Check_Param_Empty(dest_file_path);

		std::error_code ec;

		Check_Exists(source_file_path);

		if (!createDir(dest_file_path))
			return false;

		fs::copy(source_file_path, dest_file_path, fs::copy_options::overwrite_existing | fs::copy_options::recursive, ec);
		if (ec)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", source_path:" << source_file_path
				<< ", dest_path:" << dest_file_path;
			return false;
		}

		return true;
	}

	bool renamePath(const fs::path& source_file_path, const fs::path& dest_file_path)
	{
		Check_Param_Empty(source_file_path);
		Check_Param_Empty(dest_file_path);

		std::error_code ec;
		Check_Exists(source_file_path);

		auto new_dest_path = dest_file_path;
		removeEndSlash(new_dest_path); //去除结尾的/或者\

		if (!createDir(new_dest_path.parent_path()))
			return false;

		fs::rename(source_file_path, new_dest_path, ec);
		if (ec)
		{
			LOG(ERROR) << __func__ << " failed!"
				<< ", std error code: " << ec.value()
				<< ", std error message: " << ec.message()
				<< ", source_path: " << source_file_path
				<< ", dest_path: " << new_dest_path;
			return false;
		}
		return true;
	}

	bool moveOneFile(const fs::path& source_file_path, const fs::path& dest_file_path)
	{
		Check_Param_Empty(source_file_path);
		Check_Param_Empty(dest_file_path);

		std::error_code ec;

		Check_Exists(source_file_path);

		Check_Is_Simple_File(source_file_path);

		auto real_dest_file_path = dest_file_path;
		if (isDirector(dest_file_path))
		{
			real_dest_file_path = dest_file_path / source_file_path.filename();
		}

		if (!renamePath(source_file_path, real_dest_file_path))
			return false;

		return true;
	}

	bool moveDir(const fs::path& source_dir_path, const fs::path& dest_dir_path)
	{
		Check_Param_Empty(source_dir_path);
		Check_Param_Empty(dest_dir_path);

		std::error_code ec;

		Check_Exists(source_dir_path);
		Check_Is_Dir(source_dir_path);

		if (fs::is_empty(dest_dir_path, ec))
		{
			deleteDir(dest_dir_path);
		}

		if (!fs::exists(dest_dir_path, ec))
		{
			if (renamePath(source_dir_path, dest_dir_path))
				return true;
		}

		for (auto& file_ite : fs::recursive_directory_iterator(source_dir_path))
		{
			fs::path new_path = dest_dir_path / fs::relative(file_ite.path(), source_dir_path);
			if (fs::is_directory(file_ite, ec))
			{
				if (!createDir(new_path))
					return false;
			}
			else
			{
				if (fs::exists(new_path, ec))
				{
					if (!deleteOneFile(new_path))
						return false;
				}

				if (!renamePath(file_ite.path(), new_path))
					return false;
			}
		}

		return true;
	}
};