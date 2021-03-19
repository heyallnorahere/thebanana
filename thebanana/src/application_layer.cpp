#include "pch.h"
#include "application_layer.h"
#include "game.h"
namespace thebanana {
	void application_layer::register_scripts() { }
	void application_layer::init() { }
	void application_layer::gameloop() {
		this->update();
		this->render();
	}
	void application_layer::update() {
		g_game->update();
	}
	void application_layer::render() {
		g_game->render();
		g_game->swap_buffers();
	}
	void application_layer::clean_up() { }
	std::string application_layer::window_title() {
		return "banana window";
	}
}