/*=============================================================================
Copyright (C) 2016 yumetodo
https://github.com/yumetodo/win32api_prosecc_name

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include <string>
namespace win_system {
	using uint32_t = std::uint32_t;
	using uint64_t = std::uint32_t;
	using int32_t = std::int32_t;
	using int64_t = std::int64_t;
	using wstring = std::wstring;
	using size_t = std::size_t;
	struct win_system_info_elem_c {
		win_system_info_elem_c() = default;
		win_system_info_elem_c(const win_system_info_elem_c&) = default;
		win_system_info_elem_c(win_system_info_elem_c&&) = default;
		win_system_info_elem_c& operator=(const win_system_info_elem_c&) = default;
		win_system_info_elem_c& operator=(win_system_info_elem_c&&) = default;
		win_system_info_elem_c(uint64_t numof_thread, wstring name, int32_t priority, uint64_t pid, uint32_t parent_pid, uint32_t handle_num, size_t peak_pagefile_usage, size_t private_page_num);
		uint64_t numof_thread;
		wstring name;
		int32_t priority;
		uint64_t pid;
		uint32_t parent_pid;
		uint32_t handle_num;
		size_t peak_pagefile_usage;
		size_t private_page_num;
	};
	class win_system_info_c {
	public:
		win_system_info_c();
		explicit win_system_info_c(const win_system_info_c&) = delete;
		explicit win_system_info_c(win_system_info_c&& o) = default;
		win_system_info_c& operator=(const win_system_info_c&) = delete;
		win_system_info_c& operator=(win_system_info_c&&) = delete;
		win_system_info_c& update();
		std::vector<win_system_info_elem_c> info;
	private:
		void* NtQuerySystemInformation;//type PFN_NT_QUERY_SYSTEM_INFORMATION
	};
}
