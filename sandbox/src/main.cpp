#define BANANA_MAIN
#include <thebanana.h>
#include "player.h"
#include "camera.h"
#include "../resource.h"
std::string waluigi_paths(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "Waluigi\\Waluigi\\", "textures\\placeholder\\waluigi\\");
}
std::string test_texture_path(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "3dplatformer\\3dplatformer\\", "");
}
class thebanana_test_application_layer : public thebanana::application_layer {
public:
	virtual void register_scripts() override;
	virtual void init() override;
	virtual void gameloop() override;
private:
};
thebanana::application_layer* create_application_layer() {
	return new thebanana_test_application_layer;
}
void thebanana_test_application_layer::register_scripts() {
	// register scripts
	thebanana::g_game->get_script_registry()->register_script<player_behavior>();
	thebanana::g_game->get_script_registry()->register_script<camera_behavior>();
}
void thebanana_test_application_layer::init() {
#ifdef _DEBUG
	// initialize imgui debug menus
	thebanana::g_game->init_debug_menus();
#endif
	// load shaders
	thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT));
	thebanana::g_game->get_shader_registry()->register_shader("2d", new opengl_shader_library::win32_resource_shader(IDR_2D_VERTEX, IDR_2D_FRAGMENT));
	thebanana::g_game->get_scene()->set_shader_name("basic");
	thebanana::graphics::opengl::opengl_quad::init_shader("2d");
	// add gameobjects to the scene
	thebanana::gameobject* player = new thebanana::basic_gameobject;
	player->add_component<thebanana::native_script_component>().bind<player_behavior>();
	thebanana::gameobject* camera = new thebanana::basic_gameobject;
	camera->add_component<thebanana::native_script_component>().bind<camera_behavior>();
	camera->get_component<thebanana::native_script_component>().set_property("player", player);
	player->get_component<thebanana::native_script_component>().set_property("camera", camera);
	thebanana::g_game->get_scene()->add_object(camera);
	thebanana::g_game->get_scene()->add_object(player);
	thebanana::gameobject* p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(2.f, 0.75f, 2.f);
	p->add_tag("test");
	p->get_component<thebanana::rigidbody>().set_collision_tags({ "ground" }).set_collider_type<thebanana::mlfarrel_model>().set_radius(0.5f).set_origin_offset(glm::vec3(0.f, 0.5f, 0.f));
	p->get_component<thebanana::rigidbody>().set_property("mass", 1.5f);
	thebanana::g_game->get_scene()->add_object(p);
	p = new thebanana::static_mesh("test_cube");
	p->get_transform().scale(1.f, 1.f, 2.f);
	p->get_transform().translate(-5.f, 0.75f, -5.f);
	thebanana::g_game->get_scene()->add_object(p);
	// this was commented out because of buggy collision
	p = new thebanana::static_mesh("test_Lblock");
	p->get_transform().translate(10.f, 0.f, 10.f);
	p->get_transform().scale(0.5f);
	p->add_tag("ground");
	thebanana::g_game->get_scene()->add_object(p);
	p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(0.f, -1.f, 0.f);
	p->get_transform().scale(100.f, 1.f, 100.f);
	p->add_tag("ground");
	thebanana::g_game->get_scene()->add_object(p);
	// load models
	thebanana::g_game->add_model_desc({ "player", "models/placeholder/waluigi.fbx", waluigi_paths, thebanana::transform().scale(0.0005f) });
	thebanana::g_game->add_model_desc({ "test_cube", "models/cube.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_Lblock", "models/Lblock.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->load_models();
	// load sounds
	thebanana::g_game->get_sound_manager()->load_sound("click", "sounds/click.ogg");
	// load a menu
	thebanana::g_game->get_menu_manager()->load_menu(new thebanana::ui::menu("test_menu.json"));
}
void thebanana_test_application_layer::gameloop() {
	thebanana::g_game->update();
	thebanana::g_game->render();
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
