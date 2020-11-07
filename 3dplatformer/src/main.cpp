#include "pch.h"
#define BANANA_MAIN
#include "thebanana.h"
#include "player.h"
#include "camera.h"
std::string waluigi_paths(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "Waluigi\\Waluigi\\", "textures\\placeholder\\waluigi\\");
}
std::string test_texture_path(const std::string& path, void*) {
	return thebanana::model_registry::path_helper(path, "3dplatformer\\3dplatformer\\", "");
}
class thebanana_test_application_layer : public thebanana::application_layer {
public:
	virtual void init() override;
private:
	player* m_player;
	camera* m_camera;
};
thebanana::application_layer* create_application_layer() {
	return new thebanana_test_application_layer;
}
class sirpogalot : public thebanana::script {
public:
	sirpogalot(thebanana::gameobject* obj) : script(obj) { }
	virtual void update() override {
		thebanana::debug::log_print("pog");
	}
};
void thebanana_test_application_layer::init() {
	this->m_player = new player;
	this->m_camera = new camera(this->m_player);
	this->m_player->set_camera(this->m_camera);
	thebanana::g_game->get_scene()->add_object(this->m_camera);
	thebanana::g_game->get_scene()->add_object(this->m_player);
	thebanana::gameobject* p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(2.f, 0.75f, 2.f);
	p->add_tag("test");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_cube").set_collision_tags({ "ground" }).set_collider_type<thebanana::mlfarrel_model>().set_radius(0.5f).set_origin_offset(glm::vec3(0.f, 0.5f, 0.f));
	p->get_component<thebanana::rigidbody>().set_property("mass", 1.5f);
	thebanana::g_game->get_scene()->add_object(p);
	/*p = new thebanana::static_mesh("test_Lblock");
	p->get_transform().translate(-5.f, 0.f, -5.f);
	p->add_tag("ground");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_Lblock");
	thebanana::g_game->get_scene()->add_object(p);*/
	p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(0.f, -1.f, 0.f);
	p->get_transform().scale(100.f, 1.f, 100.f);
	p->add_tag("ground");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_cube");
	thebanana::g_game->get_scene()->add_object(p);
	p = new thebanana::basic_gameobject;
	p->add_component<thebanana::native_script_component>().bind<sirpogalot>();
	p->get_nickname() = "sir pog alot";
	thebanana::g_game->get_scene()->add_object(p);
	thebanana::g_game->add_model_desc({ "player", "models/placeholder/waluigi.fbx", waluigi_paths, thebanana::transform().scale(0.0005f) });
	thebanana::g_game->add_model_desc({ "collision", "models/placeholder/collision.obj", waluigi_paths, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_cube", "models/cube.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_Lblock", "models/Lblock.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->load_models();
	thebanana::g_game->get_sound_manager()->load_sound("click", "sounds/click.ogg");
	thebanana::g_game->get_menu_manager()->load_menu(new thebanana::ui::menu("test_menu.json"));
}