#include "pch.h"
#include "ui/menu.h"
#include "game.h"
using json = nlohmann::json;
namespace glm {
	void from_json(const json& j, glm::vec4& color) {
		j["r"].get_to(color.r);
		j["g"].get_to(color.g);
		j["b"].get_to(color.b);
		j["a"].get_to(color.a);
	}
}
namespace thebanana {
	namespace ui {
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
			j["children"].get_to(n.children);
		}
		menu::menu() { }
		menu::menu(const std::string& json_file) {
			this->load_from_json_file(json_file);
		}
		void menu::load_from_json_file(const std::string& json_file) {
			std::ifstream file(json_file);
			json j;
			file >> j;
			j["nodes"].get_to(this->children);
			j["clear_color"].get_to(this->clear_color);
		}
		void menu::draw(SkCanvas* canvas) {
			canvas->clear({ this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a });
			for (auto& n : this->children) {
				this->draw_node(canvas, n);
			}
		}
		void menu::draw_node(SkCanvas* canvas, node& n) {
			RECT window_rect;
			GetWindowRect(this->g_game->get_window(), &window_rect);
			int width = window_rect.right - window_rect.left;
			int height = window_rect.bottom - window_rect.top;
			this->paint.setColor4f({ n.color.r, n.color.g, n.color.b, n.color.a });
			switch (n.type) {
			case node_type::text:
				canvas->drawString(n.text.c_str(), n.x * width, n.y * height, this->font, this->paint);
				break;
			case node_type::rectangle:
			{
				SkRect r = SkRect::MakeXYWH(n.x * width, n.y * height, n.w * width, n.h * height);
				canvas->drawRect(r, this->paint);
			}
				break;
			case node_type::ellipse:
			{
				SkRect r = SkRect::MakeXYWH(n.x * width, n.y * height, n.w * width, n.h * height);
				canvas->drawOval(r, this->paint);
			}
				break;
			}
			for (auto& n : n.children) {
				this->draw_node(canvas, n);
			}
		}
		void menu::set_game_ptr(game* g_game) {
			this->g_game = g_game;
		}
	}
}