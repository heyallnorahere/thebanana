#pragma once
using win32_string = std::basic_string<TCHAR>;
extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)