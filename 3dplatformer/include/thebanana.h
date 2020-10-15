#pragma once
#include "includelist.h"
#include "components/components.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "ui/ui.h"
#include "main_module.h"
#ifndef BANANA_MAIN_HANDLED
void init_game();
void gameloop();
void clean_up_game();
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, char* cmd_line, int cmd_show) {
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpfnWndProc = ::thebanana::game::wndproc;
	wc.lpszClassName = TEXT(BANANA_WINDOW_CLASS_NAME);
	wc.hCursor = LoadCursor(HINST_THISCOMPONENT, IDC_ARROW);
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	assert(RegisterClass(&wc));
	init_game();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		gameloop();
	}
	clean_up_game();
	return static_cast<int>(msg.wParam);
}
#endif