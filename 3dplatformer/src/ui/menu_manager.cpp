#include "pch.h"
#include "ui/menu_manager.h"
#include "ui/menu.h"
#include "game.h"
#include "graphics/texture.h"
namespace thebanana {
	namespace ui {
		menu_manager::menu_manager(game* g_game) : m_game(g_game), m_current_menu(NULL), m_canvas(NULL), m_texture(NULL) {
			this->setup_canvas();
			this->setup_texture();
		}
		void menu_manager::load_menu(menu* m) {
			delete this->m_current_menu;
			this->m_current_menu = m;
			this->m_current_menu->set_game_ptr(this->m_game);
			this->update_texture_pixels();
		}
		void menu_manager::draw() {
			if (this->m_current_menu) this->m_current_menu->draw(this->m_canvas);
			// flush the canvas
			this->m_canvas->flush();
			// every 100 frames, update the textures pixels
			if (this->m_game->get_current_frame() % 100 == 0) this->update_texture_pixels();
		}
		graphics::texture* menu_manager::get_texture() {
			return this->m_texture;
		}
		menu_manager::~menu_manager() {
			delete this->m_current_menu;
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
			sk_sp<SkData> data = this->get_canvas_data();
			SkASSERT(data);
			graphics::texture::data d;
			stbi_set_flip_vertically_on_load(true);
			d.pixels = stbi_load_from_memory((const unsigned char*)data->data(), data->size(), &d.width, &d.height, &d.channels, NULL);
			stbi_set_flip_vertically_on_load(false);
			this->m_texture = graphics::texture::create(d);
			stbi_image_free(d.pixels);
		}
		void menu_manager::destroy_texture() {
			delete this->m_texture;
		}
		sk_sp<SkData> menu_manager::get_canvas_data() {
			sk_sp<SkImage> image = this->m_surface->makeImageSnapshot();
			if (!image) return NULL;
			return image->encodeToData();
		}
		void menu_manager::update_texture_pixels() {
			sk_sp<SkData> data = this->get_canvas_data();
			SkASSERT(data);
			graphics::texture::data d;
			stbi_set_flip_vertically_on_load(true);
			d.pixels = stbi_load_from_memory((const unsigned char*)data->data(), data->size(), &d.width, &d.height, &d.channels, NULL);
			stbi_set_flip_vertically_on_load(false);
			this->m_texture->set_data(d.pixels);
			stbi_image_free(d.pixels);
		}
	}
}