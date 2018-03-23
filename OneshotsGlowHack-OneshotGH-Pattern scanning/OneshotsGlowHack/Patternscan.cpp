#include "patternscan.h"

//Get Process ID From an executable name using toolhelp32Snapshot
DWORD GetProcIDW(wchar_t * exeName)
{
	PROCESSENTRY32W procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!hSnapshot)
	{
		return 0;
	}

	procEntry.dwSize = sizeof(procEntry);

	if (!Process32FirstW(hSnapshot, &procEntry))
	{
		return 0;
	}

	do
	{
		if (!wcscmp(procEntry.szExeFile, exeName))
		{
			CloseHandle(hSnapshot);
			return procEntry.th32ProcessID;
		}

	} while (Process32NextW(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return 0;
}


//Get ModuleEntry from module name, using toolhelp32snapshot
MODULEENTRY32W GetModuleW(DWORD dwProcID, wchar_t * moduleName)
{
	MODULEENTRY32W modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32W);
		if (Module32FirstW(hSnapshot, &modEntry))
		{
			do
			{
				if (!wcscmp(modEntry.szModule, moduleName))
				{
					break;
				}
			} while (Module32NextW(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

//Internal Pattern Scan
void * PatternScan(char* base, size_t size, char* pattern, char*mask)
{
	unsigned int patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}

		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
}

//External Wrapper
void * PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, char* pattern, char*  mask)
{
	uintptr_t currentChunk = begin;
	SIZE_T bytesRead;

	char mainBuffer[8192]{};

	while (currentChunk < end)
	{
		char buffer[4096];

		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
		
		memcpy(mainBuffer + sizeof(buffer), buffer, sizeof(buffer));
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), oldprotect, &oldprotect);

		if (bytesRead == 0)
		{
			return nullptr;
		}

		void* internalAddress = PatternScan((char*)&mainBuffer, sizeof(mainBuffer), pattern, mask);
		
		memcpy(mainBuffer, buffer, sizeof(buffer));
		if (internalAddress != nullptr)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&mainBuffer;
			return (void*)(currentChunk - sizeof(buffer)  + offsetFromBuffer);
		}
		else
		{
			//advance to next chunk
				currentChunk = currentChunk + bytesRead;
		}
	}
	Beep(500, 100);
	return nullptr;
}

//Module wrapper for external pattern scan
void * PatternScanExModule(HANDLE hProcess, wchar_t * exeName, wchar_t* module, char* pattern, char* mask)
{
	DWORD processID = GetProcIDW(exeName);
	MODULEENTRY32W modEntry = GetModuleW(processID, module);
	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize;
	return PatternScanEx(hProcess, begin, end, pattern, mask);
}
