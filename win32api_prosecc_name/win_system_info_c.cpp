/*=============================================================================
Copyright (C) 2016 yumetodo
https://github.com/yumetodo/win32api_prosecc_name

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "win_system_info_c.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <tchar.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
// one can also use Winternl.h if needed
//#include <Winternl.h> // for UNICODE_STRING and SYSTEM_INFORMATION_CLASS
#define STATUS_SUCCESS               ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH  ((NTSTATUS)0xC0000004L)

namespace win_system {
	namespace detail {
		typedef enum _SYSTEM_INFORMATION_CLASS {
			SystemProcessInformation = 5
		} SYSTEM_INFORMATION_CLASS;

		typedef struct _UNICODE_STRING {//Ntddk.h
			USHORT Length;
			USHORT MaximumLength;
			PWSTR  Buffer;
		} UNICODE_STRING;

		typedef LONG KPRIORITY; // Thread priority

		typedef struct _SYSTEM_PROCESS_INFORMATION_DETAILD {
			ULONG NextEntryOffset;
			ULONG NumberOfThreads;
			LARGE_INTEGER SpareLi1;
			LARGE_INTEGER SpareLi2;
			LARGE_INTEGER SpareLi3;
			LARGE_INTEGER CreateTime;
			LARGE_INTEGER UserTime;
			LARGE_INTEGER KernelTime;
			UNICODE_STRING ImageName;
			KPRIORITY BasePriority;
			HANDLE UniqueProcessId;
			ULONG InheritedFromUniqueProcessId;
			ULONG HandleCount;
			BYTE Reserved4[4];
			PVOID Reserved5[11];
			SIZE_T PeakPagefileUsage;
			SIZE_T PrivatePageCount;
			LARGE_INTEGER Reserved6[6];
		} SYSTEM_PROCESS_INFORMATION_DETAILD, *PSYSTEM_PROCESS_INFORMATION_DETAILD;
		std::string to_hex_string(NTSTATUS status) {
			std::stringstream ss;
			ss << std::hex << status;
			return ss.str();
		}
	}
	using namespace win_system::detail;
	typedef NTSTATUS(WINAPI *PFN_NT_QUERY_SYSTEM_INFORMATION)(
		IN       SYSTEM_INFORMATION_CLASS SystemInformationClass,
		IN OUT   PVOID SystemInformation,
		IN       ULONG SystemInformationLength,
		OUT OPTIONAL  PULONG ReturnLength
	);
	win_system_info_c::win_system_info_c()
		: info()
	{
		this->NtQuerySystemInformation = reinterpret_cast<void*>(GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQuerySystemInformation"));
		this->update();
	}
	win_system_info_c & win_system_info_c::update()
	{
		ULONG ReturnLength;
		ULONG bufferSize;
		reinterpret_cast<PFN_NT_QUERY_SYSTEM_INFORMATION>(NtQuerySystemInformation)(SystemProcessInformation, nullptr, 0, &ReturnLength);
		bufferSize = static_cast<size_t>(ReturnLength + ReturnLength / 2);
		auto pspid = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION_DETAILD>(new char[bufferSize]);
		for (
			NTSTATUS status;
			STATUS_SUCCESS != (status = reinterpret_cast<PFN_NT_QUERY_SYSTEM_INFORMATION>(NtQuerySystemInformation)(SystemProcessInformation, static_cast<void*>(pspid), bufferSize, &ReturnLength));
		) {
			if (STATUS_INFO_LENGTH_MISMATCH == status) throw std::runtime_error("In function win_system_info_c::update : NtQuerySystemInformation return 0x" + to_hex_string(status));
			bufferSize += ReturnLength;
			delete[] pspid;
			pspid = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION_DETAILD>(new char[bufferSize]);
		}
		
		std::vector<win_system_info_elem_c> re;
		for (;; pspid = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION_DETAILD>(pspid->NextEntryOffset + reinterpret_cast<PBYTE>(pspid))) {
			re.emplace_back(
				static_cast<uint64_t>(pspid->NumberOfThreads), std::wstring((pspid->ImageName.Length && pspid->ImageName.Buffer) ? pspid->ImageName.Buffer : L""),
				static_cast<uint32_t>(pspid->BasePriority), *(reinterpret_cast<uint64_t*>(&(pspid->UniqueProcessId))), 
				static_cast<uint32_t>(pspid->InheritedFromUniqueProcessId), 
				static_cast<uint32_t>(pspid->HandleCount), static_cast<size_t>(pspid->PeakPagefileUsage), static_cast<size_t>(pspid->PrivatePageCount)
			);
			if (pspid->NextEntryOffset == 0) break;
		}
		this->info = std::move(re);
		return *this;
	}
	win_system_info_elem_c::win_system_info_elem_c(uint64_t numof_thread, wstring name, int32_t priority, uint64_t pid, uint32_t parent_pid, uint32_t handle_num, size_t peak_pagefile_usage, size_t private_page_num)
		:numof_thread(numof_thread), name(name), priority(priority), pid(pid), parent_pid(parent_pid), handle_num(handle_num), peak_pagefile_usage(peak_pagefile_usage), private_page_num(private_page_num)	{}
}
