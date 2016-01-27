/*=============================================================================
Copyright (C) 2016 yumetodo
https://github.com/yumetodo/win32api_prosecc_name

Distributed under the Boost Software License, Version 1.0.
(See http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "win_system_info_c.hpp"
#include <iostream>
int main() {
	win_system::win_system_info_c sysinfo{};
	for (const win_system::win_system_info_elem_c& v : sysinfo.info) std::wcout << v.name << std::endl;
}
