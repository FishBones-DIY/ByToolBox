#pragma once
#include <vector>
#include <windows.h>

namespace process_utils
{
    // ��������(ִ��exe����bat��ȡ����ֵ)
    // @app_path ����ȫ·��
    // @param �������
    // @show_ui �Ƿ�չʾui
    // @error_code ����ʧ�ܴ�����::GetLastError()
    // @wait_for_exit �Ƿ�ȴ������˳�
    // @exit_code �����˳���
    // @wait_millisecond Ϊ@wait_for_exit�����ã��Ժ���Ϊ��λ��Ĭ��һֱ�ȴ��������˳�
    bool runProcess(
        const wchar_t* app_path, const wchar_t* param, bool show_ui,
        uint32_t& error_code, bool wait_for_exit, DWORD* exit_code,
        time_t wait_millisecond = INFINITE);
    bool KillProcess(const wchar_t* process_name);
    bool FindProcess(const wchar_t* process_name, std::vector<DWORD>& process_id);

    bool EnablePrivilege();
    HANDLE GetHandleByProcessId(DWORD process_id);
};

