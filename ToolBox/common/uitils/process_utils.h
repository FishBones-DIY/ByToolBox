#pragma once
#include <vector>
#include <windows.h>

namespace process_utils
{
    // 启动进程(执行exe或者bat获取返回值)
    // @app_path 程序全路径
    // @param 程序参数
    // @show_ui 是否展示ui
    // @error_code 运行失败错误码::GetLastError()
    // @wait_for_exit 是否等待程序退出
    // @exit_code 程序退出码
    // @wait_millisecond 为@wait_for_exit的设置，以毫秒为单位，默认一直等待到程序退出
    bool runProcess(
        const wchar_t* app_path, const wchar_t* param, bool show_ui,
        uint32_t& error_code, bool wait_for_exit, DWORD* exit_code,
        time_t wait_millisecond = INFINITE);
    bool KillProcess(const wchar_t* process_name);
    bool FindProcess(const wchar_t* process_name, std::vector<DWORD>& process_id);

    bool EnablePrivilege();
    HANDLE GetHandleByProcessId(DWORD process_id);
};

