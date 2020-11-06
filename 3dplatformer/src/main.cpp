#include "pch.h"
#define BANANA_MAIN
#include "thebanana.h"
std::string path_helper(const std::string& original, const std::string& find, const std::string& replace) {
	size_t pos = std::string::npos;
	std::string result = original;
	do {
		if (pos != std::string::npos) {
			result = replace + original.substr(pos + find.length());
		}
		pos = original.find(find, pos + 1);
	} while (pos != std::string::npos);
	return result;
}
std::string waluigi_paths(const std::string& path, void*) {
	return path_helper(path, "Waluigi\\Waluigi\\", "textures\\placeholder\\waluigi\\");
}
std::string kunai_paths(const std::string& path, void*) {
	return path_helper(path, "Ibukis Kunai\\SFV_IBUKI_Kunai\\", "textures\\placeholder\\kunai\\");
}
std::string mk8rr_paths(const std::string& path, void*) {
	return path_helper(path, "Wii U - Mario Kart 8 - Rainbow Road\\", "textures\\placeholder\\mk8rr\\");
}
std::string mk64rr_paths(const std::string& path, void*) {
	return path_helper(path, "Mario Kart 64 - Rainbow Road\\", "textures\\placeholder\\mk64rr\\");
}
std::string results_stage_paths(const std::string& path, void*) {
	return path_helper(path, "Results Stage\\Results Stage\\images\\", "textures\\placeholder\\resultsstage\\");
}
std::string test_texture_path(const std::string& path, void*) {
	return path_helper(path, "3dplatformer\\3dplatformer\\", "");
}
void init_game() {
	thebanana::g_game = new thebanana::game("banana window");
	thebanana::static_mesh* p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(2.f, 0.75f, 2.f);
	p->add_tag("test");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_cube").set_collision_tags({ "ground" }).set_collider_type<thebanana::mlfarrel_model>().set_radius(0.5f).set_origin_offset(glm::vec3(0.f, 0.5f, 0.f));
	p->get_component<thebanana::rigidbody>().set_property("mass", 1.5f);
	thebanana::g_game->get_scene()->add_object(p);
	p = new thebanana::static_mesh("test_Lblock");
	p->get_transform().translate(-5.f, 0.f, -5.f);
	p->get_transform().scale(0.5f);
	p->add_tag("ground");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_Lblock");
	thebanana::g_game->get_scene()->add_object(p);
	p = new thebanana::static_mesh("test_cube");
	p->get_transform().translate(0.f, -1.f, 0.f);
	p->get_transform().scale(100.f, 1.f, 100.f);
	p->add_tag("ground");
	p->add_component<thebanana::rigidbody>().set_collision_model_name("test_cube");
	thebanana::g_game->get_scene()->add_object(p);
	thebanana::g_game->add_model_desc({ "player", "models/placeholder/waluigi.fbx", waluigi_paths, thebanana::transform().scale(0.0005f) });
	thebanana::g_game->add_model_desc({ "collision", "models/placeholder/collision.obj", waluigi_paths, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_cube", "models/cube.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->add_model_desc({ "test_Lblock", "models/Lblock.obj", test_texture_path, thebanana::transform() });
	thebanana::g_game->load_models();
	thebanana::g_game->get_sound_manager()->load_sound("click", "sounds/click.ogg");
	thebanana::g_game->get_menu_manager()->load_menu(new thebanana::ui::menu("test_menu.json"));
}
void gameloop() {
	thebanana::g_game->update();
	thebanana::g_game->render();
}
void clean_up_game() {
	delete thebanana::g_game;
}