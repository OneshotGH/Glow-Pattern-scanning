#pragma once

#include <Windows.h>
#include <TlHelp32.h>

//Get Process ID From an executable name using toolhelp32Snapshot
DWORD GetProcIDW(wchar_t * exeName);

//Get ModuleEntry from module name, using toolhelp32snapshot
MODULEENTRY32W GetModuleW(DWORD dwProcID, wchar_t * moduleName);

//Internal Pattern Scan
void * PatternScan(char* base, size_t size, char* pattern, char*mask);

//External Wrapper
void * PatternScanEx(HANDLE hPRocess, uintptr_t begin, uintptr_t end, char* pattern, char*  mask);

//Module wrapper for external pattern scan
void * PatternScanExModule(HANDLE hProcess, wchar_t * exeName, wchar_t* module, char* pattern, char* mask);