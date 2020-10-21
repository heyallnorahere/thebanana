#include "pch.h"
#include "ui/menu_manager.h"
#include "ui/menu.h"
#include "game.h"
#include "graphics/texture.h"
#include "lua_interpreter.h"
#include <stb_image.h>
namespace thebanana {
	namespace ui {
		menu_manager::menu_manager(game* g_game) : m_game(g_game), m_current_menu(NULL), m_canvas(NULL), m_texture(NULL), m_draw_menus(false) {
			this->setup_canvas();
			this->setup_texture();
			this->m_interpreter = new lua_interpreter;
		}
		void menu_manager::load_menu(menu* m) {
			delete this->m_current_menu;
			this->m_current_menu = m;
			this->m_current_menu->set_ptrs(this->m_game, this->m_interpreter);
			this->update_texture_pixels();
		}
		void menu_manager::update() {
			if (this->m_current_menu) this->m_current_menu->update();
		}
		void menu_manager::draw() {
			if (this->m_draw_menus && this->m_current_menu) {
				// if the menus should be drawn, and there is a menu loaded, draw the menu
				this->m_current_menu->draw(this->m_canvas);
			}
			// flush the canvas
			this->m_canvas->flush();
			// update the textures pixels
			this->update_texture_pixels();
		}
		graphics::texture* menu_manager::get_texture() {
			return this->m_texture;
		}
		menu_manager::~menu_manager() {
			delete this->m_current_menu;
			delete this->m_interpreter;
			this->destroy_texture();
		}
		void menu_manager::update_canvas_size() {
			RECT r;
			GetClientRect(this->m_game->get_window(), &r);
			this->m_canvas->scale(r.right, r.bottom);
		}
		void menu_manager::setup_canvas() {
			RECT window_rect;
			GetWindowRect(this->m_game->get_window(), &window_rect);
			int width = abs(window_rect.right - window_rect.left);
			int height = abs(window_rect.bottom - window_rect.top);
			this->m_surface = SkSurface::MakeRasterN32Premul(width, height);
			this->m_canvas = this->m_surface->getCanvas();
			this->update_canvas_size();
		}
		void menu_manager::setup_texture() {
			graphics::texture::data d;
			d.pixels = NULL;
			const SkImageInfo& info = this->m_surface->imageInfo();
			d.channels = info.bytesPerPixel();
			d.width = info.width();
			d.height = info.height();
			this->m_texture = graphics::texture::create(d);
		}
		void menu_manager::destroy_texture() {
			delete this->m_texture;
		}
		void menu_manager::update_texture_pixels() {
			const SkImageInfo& info = this->m_surface->imageInfo();
			unsigned char* buffer = (unsigned char*)malloc(static_cast<size_t>(info.width() * info.height() * info.bytesPerPixel()));
			assert(buffer);
			SkPixmap pixmap(info, buffer, static_cast<size_t>(info.width() * info.bytesPerPixel()));
			this->m_surface->readPixels(pixmap, 0, 0);
			this->m_texture->set_data(buffer);
			free(buffer);
		}
		void menu_manager::open_menus() {
			this->m_draw_menus = true;
			this->m_game->show_cursor();
		}
		void menu_manager::close_menus() {
			this->m_draw_menus = false;
			this->m_game->hide_cursor();
		}
		void menu_manager::toggle_menus() {
			this->m_draw_menus = !this->m_draw_menus;
			this->m_game->showing_cursor() = this->m_draw_menus;
		}
		bool menu_manager::menus_open() {
			return this->m_draw_menus;
		}
	}
}