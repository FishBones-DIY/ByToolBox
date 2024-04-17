#pragma once

#ifndef GLOG_NO_ABBREVIATED_SEVERITIES
#define GLOG_NO_ABBREVIATED_SEVERITIES
#endif // !GLOG_NO_ABBREVIATED_SEVERITIES

// MT和MTD时开启
// #ifndef GOOGLE_GLOG_DLL_DECL
// #define GOOGLE_GLOG_DLL_DECL
// #endif // !GOOGLE_GLOG_DLL_DECL

#include "3rd/glog/md/logging.h"

#ifdef _DEBUG
#pragma comment(lib, "3rd/glog/md/lib/glogd.lib")
#else
#pragma comment(lib, "3rd/glog/md/lib/glog.lib")
#endif

inline std::ostream& operator<<(std::ostream& out, const wchar_t* str) {
#pragma warning(push)
#pragma warning(disable : 4996)
	size_t len = wcsrtombs(NULL, &str, 0, NULL);
	// 不支持编码的场景下会返回-1，比如纯英文操作系统下的中文字符
	if (len < 0 || len == 0xffffffff) {
		return out;
	}
	char* buf = (char*)malloc(len + 1);
	buf[len] = 0;
	wcsrtombs(buf, &str, len, NULL);
#pragma warning(pop)
	out << buf;
	free(buf);
	return out;
}

inline std::ostream& operator<<(std::ostream& out, const std::wstring& str) {
	return operator<<(out, str.c_str());
}

namespace smart_glog 
{
	bool initLogger(const char* module_name,const char* log_path);
	void shutdownLogger();

} // namespace toolbox 