#pragma once
#include "includelist.h"
#include "components/components.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "ui/ui.h"
#include "sound/sound.h"
#include "main_module.h"
#ifdef BANANA_MAIN
::thebanana::application_layer* create_application_layer();
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, char* cmd_line, int cmd_show) {
	::thebanana::application_layer* app_layer = create_application_layer();
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpfnWndProc = ::thebanana::game::wndproc;
	wc.lpszClassName = TEXT(BANANA_WINDOW_CLASS_NAME);
	wc.hCursor = LoadCursor(HINST_THISCOMPONENT, IDC_ARROW);
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	assert(RegisterClass(&wc));
	::thebanana::g_game = new ::thebanana::game(app_layer->window_title());
	app_layer->register_scripts();
	app_layer->init();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		app_layer->gameloop();
	}
	app_layer->clean_up();
	delete ::thebanana::g_game;
	delete app_layer;
	return static_cast<int>(msg.wParam);
}
#endif//BANANA_MAIN