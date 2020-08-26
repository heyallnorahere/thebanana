#pragma once
using win32_string = std::basic_string<TCHAR>;
extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#define CURRENT_TIME(type) (std::chrono::duration_cast<std::chrono::duration<type>>(std::chrono::system_clock::now().time_since_epoch()).count())