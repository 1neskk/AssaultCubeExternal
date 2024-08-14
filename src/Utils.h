#pragma once

#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

namespace Utils
{
    DWORD GetProcessId(const WCHAR* processName);
    DWORD GetModuleAddress(DWORD dwPid, const WCHAR* moduleName);

    template <class T>
    T ReadMemory(HANDLE hProcess, DWORD dwAddress)
    {
        T buffer;
        ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(dwAddress), &buffer, sizeof(T), nullptr);
        return buffer;
    }

    template <class T>
    void WriteMemory(HANDLE hProcess, DWORD dwAddress, T value)
    {
        WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(dwAddress), &value, sizeof(T), nullptr);
    }
}