#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>

namespace Memory
{
    DWORD GetProcessId(const WCHAR* processName);
    DWORD GetModuleAddress(DWORD dwPid, const WCHAR* moduleName);

	void PatchEx(BYTE* dst, const BYTE* src, const uint32_t size, const HANDLE hProcess);
    void NopEx(BYTE* dst, uint32_t size, HANDLE hProcess);

    template <class T>
    T ReadMemory(HANDLE hProcess, DWORD dwAddress)
    {
        T buffer;
        ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(dwAddress), &buffer, sizeof(T), nullptr);
        return buffer;
    }

	inline void ReadString(HANDLE hProcess, DWORD dwAddress, char* buffer)
	{
		ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(dwAddress), buffer, sizeof(buffer), nullptr);
	}

    template <class T>
    void WriteMemory(HANDLE hProcess, DWORD dwAddress, T value)
    {
        WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(dwAddress), &value, sizeof(T), nullptr);
    }
}