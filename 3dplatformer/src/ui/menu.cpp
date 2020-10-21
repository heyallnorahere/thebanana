#include "pch.h"
#include "ui/menu.h"
#include "game.h"
#include "lua_interpreter.h"
#include "input_manager.h"
#include "mouse.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace glm {
	void from_json(const json& j, glm::vec4& color) {
		j["r"].get_to(color.r);
		j["g"].get_to(color.g);
		j["b"].get_to(color.b);
		j["a"].get_to(color.a);
	}
}
bool exists(const json& j, const std::string& name) {
	return j.find(name) != j.end();
}
namespace thebanana {
	namespace ui {
		menu* currently_loading_menu = NULL;
		void from_json(const json& j, menu::node& n) {
			std::string type_name = j["node_type"].get<std::string>();
			if (type_name == "label" || type_name == "text") n.type = menu::node_type::text;
			else if (type_name == "rectangle" || type_name == "rect") n.type = menu::node_type::rectangle;
			else if (type_name == "ellipse") n.type = menu::node_type::ellipse;
			else n.type = menu::node_type::other;
			j["x"].get_to(n.x);
			j["y"].get_to(n.y);
			j["w"].get_to(n.w);
			j["h"].get_to(n.h);
			j["text"].get_to(n.text);
			j["color"].get_to(n.color);
			n.onclick = "no_function";
			if (currently_loading_menu) {
				if (currently_loading_menu->script_loaded() && exists(j, "onclick")) {
					j["onclick"].get_to(n.onclick);
				}
			}
			j["children"].get_to(n.children);
		}
		void menu::on_click() {
			glm::vec2 cursor_pos(0.5f);
			switch (this->g_game->get_input_manager()->get_device_type(0)) {
			case input_manager::device_type::keyboard:
			{
				POINT p;
				GetCursorPos(&p);
				RECT r;
				GetWindowRect(this->g_game->get_window(), &r);
				p.x -= r.left;
				p.y -= r.top;
				cursor_pos = glm::vec2(static_cast<float>(p.x) / (r.right - r.left), static_cast<float>(p.y) / (r.bottom - r.top));
			}
				break;
			}
			node* n = NULL;
			for (auto& c : this->children) {
				c.get_top_node_at_pos(n, cursor_pos);
			}
			if (n) {
				if (n->onclick != "no_function") {
					this->interpreter->call_function(n->onclick, std::vector<lua_interpreter::param_type>());
				}
			}
		}
		menu::menu() {
			this->font.setTypeface(SkTypeface::MakeDefault());
			this->paint.setAntiAlias(true);
			this->has_script = false;
		}
		menu::menu(const std::string& json_file) : menu() {
			this->load_from_json_file(json_file);
		}
		menu::~menu() {
			if (this->has_script) {
				this->interpreter->call_function("on_unload", std::vector<lua_interpreter::param_type>());
			}
		}
		void menu::load_from_json_file(const std::string& json_file) {
			currently_loading_menu = this;
			std::ifstream file(json_file);
			json j;
			file >> j;
			if (exists(j, "script")) {
				j["script"].get_to(this->script_path);
				this->has_script = true;
			}
			else {
				this->has_script = false;
			}
			j["nodes"].get_to(this->children);
			j["clear_color"].get_to(this->clear_color);
		}
		void menu::update() {
			if (this->g_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
				mouse* m = (mouse*)this->g_game->get_input_manager()->get_device(1);
				std::vector<input_manager::device::button> btns = m->get_buttons();
				if (btns[0].down) {
					this->on_click();
				}
			}
		}
		void menu::draw(SkCanvas* canvas) {
			canvas->clear({ this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a });
			for (auto& n : this->children) {
				this->draw_node(canvas, n);
			}
		}
		void menu::draw_node(SkCanvas* canvas, node& n) {
			this->paint.setColor4f({ n.color.r, n.color.g, n.color.b, n.color.a });
			switch (n.type) {
			case node_type::text:
				this->font.setSize(n.h);
				canvas->drawString(n.text.c_str(), n.x, n.y, this->font, this->paint);
				break;
			case node_type::rectangle:
			{
				SkRect r = SkRect::MakeXYWH(n.x, n.y, n.w, n.h);
				canvas->drawRect(r, this->paint);
			}
			break;
			case node_type::ellipse:
			{
				SkRect r = SkRect::MakeXYWH(n.x, n.y, n.w, n.h);
				canvas->drawOval(r, this->paint);
			}
			break;
			}
			for (auto& n : n.children) {
				this->draw_node(canvas, n);
			}
		}
		void menu::set_ptrs(game* g_game, lua_interpreter* interpreter) {
			this->g_game = g_game;
			this->interpreter = interpreter;
			if (this->has_script) {
				this->interpreter->open_file(this->script_path);
				this->interpreter->call_function("on_load", std::vector<lua_interpreter::param_type>());
			}
		}
		bool menu::script_loaded() {
			return this->has_script;
		}
		void menu::node::get_top_node_at_pos(node*& ptr, glm::vec2 cursor_pos) {
			switch (this->type) {
			case node_type::rectangle:
			{
				bool x = (this->x < cursor_pos.x) && (this->x + this->w > cursor_pos.x);
				bool y = (this->y < cursor_pos.y) && (this->y + this->h > cursor_pos.y);
				if (x && y) {
					ptr = this;
				}
			}
			break;
			case node_type::ellipse:
			{
				glm::vec2 local_pos = (cursor_pos - glm::vec2(this->x, this->y)) / glm::vec2(this->w, this->h);
				if (local_pos.x < 0.f || local_pos.y < 0.f) break;
				if (local_pos.x > 1.f || local_pos.y > 1.f) break;
				glm::vec2 relative_pos = local_pos - 0.5f;
				float angle = atan2(relative_pos.y, relative_pos.x);
				float limit_y = sin(angle) * 0.5f;
				float limit_x = sin(angle) * 0.5f;
				if ((fabs(relative_pos.x) > fabs(limit_x)) && (fabs(relative_pos.y) > fabs(limit_y))) {
					break;
				}
				ptr = this;
			}
				break;
			}
			for (auto& c : this->children) {
				c.get_top_node_at_pos(ptr, cursor_pos);
			}
		}
	}
}