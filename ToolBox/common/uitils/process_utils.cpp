#include <Shlwapi.h>
#include <tlhelp32.h>

#include <string>

#include "common/public/log.h"

#include "process_utils.h"

#pragma comment(lib,"Shlwapi.lib")

//#include "Public/Helper/StringUtils.h"

namespace process_utils
{
	bool runProcess(
		const wchar_t* app_path, const wchar_t* param, bool show_ui,
		uint32_t& error_code, bool wait_for_exit, DWORD* exit_code,
		time_t wait_millisecond /*= INFINITE*/)
	{
		error_code = 0;

		if (::PathIsRelative(app_path)) return false;
		if (::PathFileExists(app_path) == FALSE) return false;

		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };

		si.cb = sizeof(si);
		GetStartupInfo(&si);

		if (show_ui)
		{
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOW;
		}
		else
		{
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
		}

		std::wstring app_name_temp(app_path);
		//Base::String::trim(app_name_temp);		// 删除两头的空格

		// 最短只接受类似 a.exe 这样的命令，至少5个字符
		if (app_name_temp.length() < 5) return false;

		std::wstring cmd_line;

		if (app_name_temp[0] == '\"')
		{
			// 说明头上已经有了引号了
			cmd_line = app_name_temp;
		}
		else
		{
			// 头上如果没有引号，自动加上引号(即使尾巴上有引号也加）
			cmd_line += L'\"';
			cmd_line += app_name_temp;
			cmd_line += L'\"';
		}

		int app_path_len = (int)app_name_temp.length();
		int param_len = param ? (int)wcslen(param) : 0;

		// 如果路径和参数太长，不启动
		if (app_path_len + param_len >= 32700) return false;

		if (param_len > 0)
		{

			cmd_line += L" ";
			cmd_line += param;
		}

		bool ret = ::CreateProcess(NULL, (LPWSTR)cmd_line.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi);

		error_code = ::GetLastError();

		if (ret)
		{
			if (wait_for_exit != 0)
			{
				auto status = ::WaitForSingleObject(pi.hProcess, wait_millisecond);
				if (status == WAIT_TIMEOUT)
				{
					LOG(ERROR) << "Timeout waiting for process to exit, second:" << wait_millisecond / 1000 << " , handal:" << pi.hProcess;
					error_code = WAIT_TIMEOUT;
					ret = false;
				}
				else
					::GetExitCodeProcess(pi.hProcess, exit_code);
			}
		}

		if (NULL != pi.hThread && INVALID_HANDLE_VALUE != pi.hThread)
			CloseHandle(pi.hThread);
		if (NULL != pi.hProcess && INVALID_HANDLE_VALUE != pi.hProcess)
			CloseHandle(pi.hProcess);

		return ret;
	}

	bool KillProcess(const wchar_t* process_name)
	{
		std::vector<DWORD> process_handle_vec;
		FindProcess(process_name, process_handle_vec);

		//EnablePrivilege();
		DWORD desired_access = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE | SYNCHRONIZE;
		bool ret = true;
		for (auto process_id : process_handle_vec)
		{
			HANDLE process_handle = ::OpenProcess(desired_access, FALSE, process_id);
			if (process_handle == NULL)
			{
				ret = false;
				break;
			}

			if (TerminateProcess(process_handle, 1111))	//杀死进程
			{
				::WaitForSingleObject(process_handle, INFINITE);
			}
			else
			{
				ret = false;
			}

			::CloseHandle(process_handle);
		}
		return ret;
	}

	bool FindProcess(const wchar_t* process_name, std::vector<DWORD>& process_id)
	{
		process_id.clear();

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	// 拍摄快照 
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return false;

		PROCESSENTRY32 ps = { 0 };
		ps.dwSize = sizeof(PROCESSENTRY32);

		bool bMore = ::Process32FirstW(hSnapshot, &ps);

		while (bMore)
		{
			if (_wcsicmp(ps.szExeFile, process_name) == 0)
			{
				process_id.push_back(ps.th32ProcessID);
			}

			bMore = ::Process32NextW(hSnapshot, &ps);
		}

		if (hSnapshot != NULL)
		{
			CloseHandle(hSnapshot);
		}

		return true;
	}

	bool EnablePrivilege()
	{
		int nResult = FALSE;
		int nRetCode = FALSE;
		HANDLE hToken = NULL;
		TOKEN_PRIVILEGES tkp = { 0 };

		do
		{
			nRetCode = ::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
			if (!nRetCode)
				break;

			nRetCode = ::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
			if (!nRetCode)
				break;

			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			nRetCode = ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
			if (!nRetCode)
				break;

			nResult = TRUE;
		} while (FALSE);

		if (hToken != NULL)
		{
			CloseHandle(hToken);
		}

		return nResult;
	}

	HANDLE GetHandleByProcessId(DWORD process_id)
	{
		return NULL;
	}
};