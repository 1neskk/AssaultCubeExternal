#include "Utils.h"

DWORD Utils::GetProcessId(const WCHAR* processName)
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

DWORD Utils::GetModuleAddress(DWORD dwPid, const WCHAR* moduleName)
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
