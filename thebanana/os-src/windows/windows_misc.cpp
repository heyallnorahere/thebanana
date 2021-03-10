#include "pch.h"
#include "platform_misc.h"
namespace thebanana {
	namespace platform_specific {
		std::string get_module_name(module_t module) {
			char buf[100];
			GetModuleFileNameA(module, buf, 100);
			return buf;
		}
		size_t get_last_os_error() {
			return (size_t)GetLastError();
		}
	}
}