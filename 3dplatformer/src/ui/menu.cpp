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
		menu::menu() {
			this->font.setTypeface(SkTypeface::MakeDefault());
		}
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
			this->paint.setColor4f({ n.color.r, n.color.g, n.color.b, n.color.a });
			canvas->save();
			canvas->translate(n.x, n.y);
			switch (n.type) {
			case node_type::text:
				this->font.setSize(n.h);
				canvas->drawString(n.text.c_str(), 0.f, 0.f, this->font, this->paint);
				break;
			case node_type::rectangle:
			{
				SkRect r = SkRect::MakeXYWH(0.f, 0.f, n.w, n.h);
				canvas->drawRect(r, this->paint);
			}
				break;
			case node_type::ellipse:
			{
				SkRect r = SkRect::MakeXYWH(0.f, 0.f, n.w, n.h);
				canvas->drawOval(r, this->paint);
			}
				break;
			}
			canvas->restore();
			for (auto& n : n.children) {
				this->draw_node(canvas, n);
			}
		}
		void menu::set_game_ptr(game* g_game) {
			this->g_game = g_game;
		}
	}
}