#include "pch.h"
#include "application_layer.h"
#include "game.h"
namespace thebanana {
	void application_layer::init() { }
	void application_layer::gameloop() {
		g_game->update();
		g_game->render();
	}
	void application_layer::clean_up() { }
	std::string application_layer::window_title() {
		return "banana window";
	}
}