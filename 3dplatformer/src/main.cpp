#include "pch.h"
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
using namespace thebanana;
void init_game() {
	g_game = new game("banana window");
	prop* p = new prop("collision");
	p->get_transform().translate(2.f, 0.75f, 2.f);
	p->add_tag("test");
	p->add_component<rigidbody>().set_collision_model_name("collision").set_collision_tags({ "ground" }).set_collider_type<mlfarrel_model>().set_radius(0.5f).set_origin_offset(glm::vec3(0.f, 0.5f, 0.f));
	p->get_component<rigidbody>().set_property("mass", 1.5f);
	g_game->get_scene()->add_object(p);
	p = new prop("collision");
	p->get_transform().translate(0.f, -1.f, 0.f);
	p->get_transform().scale(100.f, 1.f, 100.f);
	p->add_tag("ground");
	p->add_component<rigidbody>().set_collision_model_name("collision");
	g_game->get_scene()->add_object(p);
	g_game->add_model_desc({ "waluigi", "models/placeholder/waluigi.fbx", waluigi_paths, transform().scale(0.0005f) });
	g_game->add_model_desc({ "collision", "models/placeholder/collision.obj", waluigi_paths, transform() });
	g_game->load_models();
}
void gameloop() {
	g_game->update();
	g_game->render();
}
void clean_up_game() {
	delete g_game;
}