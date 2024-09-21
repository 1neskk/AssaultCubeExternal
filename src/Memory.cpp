#include "Memory.h"

DWORD Memory::GetProcessId(const WCHAR* processName)
{
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	DWORD pid = 0;

	if (Process32FirstW(snapshot, &entry))
	{
		do
		{
			if (!wcscmp(entry.szExeFile, processName))
			{
				pid = entry.th32ProcessID;
				break;
			}
		} while (Process32NextW(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return pid;
}

DWORD Memory::GetModuleAddress(DWORD dwPid, const WCHAR* moduleName)
{
	MODULEENTRY32W entry;
	entry.dwSize = sizeof(MODULEENTRY32W);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPid);

	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	DWORD baseAddress = 0;

	if (Module32FirstW(snapshot, &entry))
	{
		do
		{
			if (!wcscmp(entry.szModule, moduleName))
			{
				baseAddress = reinterpret_cast<DWORD>(entry.modBaseAddr);
				break;
			}
		} while (Module32NextW(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return baseAddress;
}

void Memory::PatchEx(BYTE* dst, const BYTE* src, const uint32_t size, const HANDLE hProcess)
{
	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}

void Memory::NopEx(BYTE* dst, uint32_t size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);
	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

