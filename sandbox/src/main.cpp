#define BANANA_MAIN
#include <thebanana.h>
#include "player.h"
#include "camera.h"
#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<> struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<> struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}
std::string waluigi_paths(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "Waluigi\\Waluigi\\", "textures\\placeholder\\waluigi\\");
}
std::string test_texture_path(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "3dplatformer\\3dplatformer\\", "");
}
class sandbox_application_layer : public thebanana::application_layer {
public:
	virtual void register_scripts() override;
	virtual void init() override;
	virtual void gameloop() override;
	virtual std::string window_title() override;
private:
};
thebanana::application_layer* create_application_layer() {
	return new sandbox_application_layer;
}
void sandbox_application_layer::register_scripts() {
	// if a script module is specified, load it
	std::vector<std::string> cmdline = thebanana::g_game->get_command_line();
	if (cmdline.size() > 3) {
		thebanana::g_game->load_script_module(cmdline[3]);
		return;
	}
	// register scripts
	thebanana::g_game->get_script_registry()->register_script<player_behavior>();
	thebanana::g_game->get_script_registry()->register_script<camera_behavior>();
}
struct { std::string find, replace; } findreplacestruct;
std::string model_loader_proc(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, findreplacestruct.find, findreplacestruct.replace);
}
void load_project(const std::string& path) {
	YAML::Node file = YAML::LoadFile(path);
	assert(file["models"]);
	for (YAML::Node m : file["models"]) {
		std::string path = m["path"].as<std::string>();
		std::string name = m["name"].as<std::string>();
		bool should_replace = m["should_replace"].as<bool>();
		if (should_replace) {
			findreplacestruct.find = m["find"].as<std::string>();
			findreplacestruct.replace = m["replace"].as<std::string>();
		}
		thebanana::g_game->get_model_registry()->load({ { name, path, should_replace ? model_loader_proc : NULL, thebanana::transform() } });
	}
	assert(file["materials"]);
	for (YAML::Node m : file["materials"]) {
		auto registry = thebanana::g_game->get_material_registry();
		thebanana::material* mat = registry->find(registry->new_material());
		mat->set_texture(m["texture"].as<std::string>());
		mat->set_normal_map(m["normal map"].as<std::string>());
		mat->set_diffuse(m["diffuse"].as<glm::vec3>());
		mat->set_specular(m["specular"].as<glm::vec3>());
		mat->set_ambient(m["ambient"].as<glm::vec3>());
		mat->set_shininess(m["shininess"].as<float>());
		mat->set_uuid(m["uuid"].as<unsigned long long>());
	}
}
void sandbox_application_layer::init() {
#ifdef _DEBUG
	// initialize imgui debug menus
	thebanana::g_game->init_debug_menus();
#endif
	// load shaders
	thebanana::g_game->get_shader_registry()->register_shader("basic", "basic");
	thebanana::g_game->get_shader_registry()->register_shader("depth", "depth", true);
	thebanana::g_game->get_shader_registry()->register_shader("2d", "2d");
	thebanana::g_game->get_scene()->set_shader_name("basic");
	thebanana::g_game->get_scene()->set_depth_shader_name("depth");
	thebanana::graphics::quad::init_opengl_shader("2d");
	// if a scene is specified, load it
	std::vector<std::string> cmdline = thebanana::g_game->get_command_line();
	bool r = false;
	if (cmdline.size() > 2) {
		if (!cmdline[2].empty()) {
			load_project(cmdline[2]);
			r = true;
		}
	}
	if (cmdline.size() > 1) {
		if (!cmdline[1].empty()) {
			thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
			serializer.deserialize(cmdline[1]);
			r = true;
		}
	}
	if (r) return;
	// load models
	thebanana::g_game->add_model_desc({ "player", "models/placeholder/waluigi.fbx", waluigi_paths, thebanana::transform().scale(0.0005f) });
	thebanana::g_game->add_model_desc({ "test_cube", "models/cube.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_Lblock", "models/Lblock.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->load_models();
	// add gameobjects to the scene
	thebanana::gameobject* player = new thebanana::basic_gameobject;
	thebanana::gameobject* camera = new thebanana::basic_gameobject;
	thebanana::g_game->get_scene()->add_object(camera);
	thebanana::g_game->get_scene()->add_object(player);
	player->add_component<thebanana::native_script_component>().bind<player_behavior>();
	camera->add_component<thebanana::native_script_component>().bind<camera_behavior>();
	camera->find_script<camera_behavior>()->get_interface()->set_property("player", player);
	player->find_script<player_behavior>()->get_interface()->set_property("camera", camera);
	thebanana::gameobject* p = new thebanana::static_mesh("test_cube");
	thebanana::g_game->get_scene()->add_object(p);
	p->get_transform().translate(2.f, 0.75f, 2.f);
	p->add_tag("test");
	p->get_component<thebanana::rigidbody>().set_property("mass", 1.5f);
	p = new thebanana::static_mesh("test_cube");
	thebanana::g_game->get_scene()->add_object(p);
	p->get_transform().scale(1.f, 1.f, 2.f);
	p->get_transform().translate(-5.f, 0.75f, -5.f);
	// this was commented out because of buggy collision
	p = new thebanana::static_mesh("test_Lblock");
	thebanana::g_game->get_scene()->add_object(p);
	p->get_transform().translate(10.f, 0.f, 10.f);
	p->get_transform().scale(0.5f);
	p->add_tag("ground");
	p = new thebanana::static_mesh("test_cube");
	thebanana::g_game->get_scene()->add_object(p);
	p->get_transform().translate(0.f, -1.f, 0.f);
	p->get_transform().scale(100.f, 1.f, 100.f);
	p->add_tag("ground");
	// load sounds
	thebanana::g_game->get_sound_manager()->load_sound("click", "sounds/click.ogg");
	// load a menu
	thebanana::g_game->get_menu_manager()->load_menu(new thebanana::ui::menu("test_menu.json"));
}
void sandbox_application_layer::gameloop() {
	thebanana::g_game->update();
	thebanana::g_game->render();
	thebanana::g_game->swap_buffers();
#ifdef _DEBUG
	if (thebanana::g_game->get_input_manager()->get_device_type(0) == thebanana::input_manager::device_type::keyboard) {
		std::vector<thebanana::input_manager::device::button> btns = thebanana::g_game->get_input_manager()->get_device_buttons(0);
		if (btns[DIK_EQUALS].down) {
			thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
			serializer.serialize("scenes/test.basket");
		}
		if (btns[DIK_MINUS].down) {
			thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
			serializer.deserialize("scenes/test.basket");
		}
	}
#endif
}
std::string sandbox_application_layer::window_title() {
	return "sandbox";
}
