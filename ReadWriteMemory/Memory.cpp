#include "Memory.h"

Memory::Memory(const std::string_view processName) : ProcessID(NULL), ProcessHandle(nullptr)
{
	::PROCESSENTRY32 entry = { };
	entry.dwSize = sizeof(::PROCESSENTRY32);

	const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	while (::Process32Next(snapShot, &entry))
	{
		if (processName.compare(entry.szExeFile))
		{
			ProcessID = entry.th32ProcessID;
			ProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
			break;
		}
	}

	if (snapShot)
		::CloseHandle(snapShot);
}

Memory::~Memory()
{
	if (ProcessHandle)
		::CloseHandle(ProcessHandle);
}

const std::uintptr_t Memory::GetModuleBaseAddress(const std::string_view moduleName)
{
	::MODULEENTRY32 entry = { };
	entry.dwSize = sizeof(::MODULEENTRY32);

	const HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, ProcessID);

std:uintptr_t result = NULL;

	while (::Module32Next(snapShot, &entry))
	{
		if (moduleName.compare(entry.szModule))
		{
			result = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			break;
		}
	}

	if (snapShot)
		::CloseHandle(snapShot);

	return result;
}

template <typename T>
constexpr const T& Read(const uintptr_t& address)
{
	T value = { };
	::ReadProcessMemory(ProcessHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
	return value;
}

template <typename T>
constexpr const T& Write(const uintptr_t& address, const T& value)
{
	T value = { };
	::WriteProcessMemory(ProcessHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
}
