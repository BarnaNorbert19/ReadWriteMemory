#pragma once

#include <cstdint>
#include <string_view>
#include <Windows.h>
#include <TlHelp32.h>

#define WIN32_LEAN_AND_MEAN

class Memory
{
private:
	uintptr_t ProcessID;
	void* ProcessHandle;

public:
	Memory(const std::string_view processName);
	~Memory();

	const std::uintptr_t GetModuleBaseAddress(const std::string_view moduleName);
};