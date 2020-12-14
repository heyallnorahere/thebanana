#include "pch.h"
#include "scene.h"
#include "game.h"
#include "input_manager.h"
#include "controller.h"
#include "mouse.h"
#include "keyboard.h"
#include "debug_tools.h"
#include "shader_registry.h"
#include "physics/rigidbody.h"
#include "components/camera_component.h"
namespace thebanana {
	scene::scene(game* g) {
		this->m_shader = this->m_depth_shader = NULL;
		this->m_game = g;
		this->set_shadow_defaults();
	}
	scene::~scene() {
		char buf[256];
		_ui64toa(this->m_children.size(), buf, 10);
		this->m_game->debug_print("deleting " + std::string(buf) + (this->m_children.size() == 1 ? " object" : " objects"));
	}
	void scene::remove_object(size_t index) {
		auto it = this->m_children.begin();
		std::advance(it, index);
		auto& obj = *it;
		this->m_children.remove(obj);
	}
	void scene::clear() {
		char buf[256];
		_ui64toa(this->m_children.size(), buf, 10);
		this->m_game->debug_print("deleting " + std::string(buf) + (this->m_children.size() == 1 ? " object" : " objects"));
		this->m_children.clear();
		this->m_game->get_rigidbody_list().clear();
	}
	void scene::update() {
#ifdef _DEBUG
		{
			bool toggle_control = false, toggle_cursor = false;
			if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
				keyboard* kb = (keyboard*)this->m_game->get_input_manager()->get_device(0);
				std::vector<keyboard::button> btns = kb->get_buttons();
				toggle_control = btns[DIK_F1].down;
				toggle_cursor = btns[DIK_F2].down;
			}
			if (toggle_control) {
				debug::control = !debug::control;
			}
			if (toggle_cursor) {
				this->m_game->toggle_cursor();
			}
		}
#endif
		for (auto& c : this->m_children) {
			c->update();
		}
	}
	void scene::render() {
		if (this->m_depth_shader) {
			int bound_framebuffer;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_framebuffer);
			opengl_shader_library::shader::use(this->m_depth_shader);
			std::vector<light_component*> lights = this->find_all_instances_of_component<light_component>();
			glm::mat4 shadow_projection = glm::perspective(glm::radians(90.f), 1.f, this->m_shadow_settings.near_plane, this->m_shadow_settings.far_plane);
			for (auto light : lights) {
				light->get_depthbuffer()->bind();
				this->m_game->clear_screen();
				auto& u = this->m_depth_shader->get_uniforms();
				auto light_data = light->get_data();
				u.vec3("lightpos", light_data.position);
				u._float("far_plane", this->m_shadow_settings.far_plane);
				std::vector<glm::mat4> shadow_transforms;
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				light->set_shadow_transforms(shadow_transforms);
				for (size_t i = 0; i < shadow_transforms.size(); i++) {
					u.mat4("shadow_matrices[" + std::to_string(i) + "]", shadow_transforms[i]);
				}
				this->render_scene();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, (unsigned int)bound_framebuffer);
		}
		opengl_shader_library::shader::use(this->m_shader);
		this->render_scene();
		opengl_shader_library::shader::use(NULL);
	}
	size_t scene::get_children_count() const {
		return this->m_children.size();
	}
	const gameobject* scene::get_child(size_t index) const {
		auto it = this->m_children.begin();
		std::advance(it, index);
		const auto& c = *it;
		return c.get();
	}
	gameobject* scene::get_child(size_t index) {
		auto it = this->m_children.begin();
		std::advance(it, index);
		const auto& c = *it;
		return c.get();
	}
	opengl_shader_library::shader* scene::get_shader() const {
		return this->m_shader;
	}
	opengl_shader_library::shader* scene::get_depth_shader() const {
		return this->m_depth_shader;
	}
	void scene::set_shader_name(const std::string& shader_name) {
		this->m_shader = this->m_game->get_shader_registry()->get_shader(shader_name);
	}
	void scene::set_depth_shader_name(const std::string& shader_name) {
		this->m_depth_shader = this->m_game->get_shader_registry()->get_shader(shader_name);
	}
	game* scene::get_game() {
		return this->m_game;
	}
	gameobject* scene::find(unsigned long long uuid) {
		gameobject* object = NULL;
		for (auto& c : this->m_children) {
			c->find(uuid, object);
			if (object) break;
		}
		return object;
	}
	static void _find_main_camera(scene* s, gameobject* object, gameobject*& pointer) {
		if (object->has_component<camera_component>()) {
			if (*object->get_component<camera_component>().get_property<bool>("Primary")) {
				pointer = object;
				return;
			}
		}
		for (size_t i = 0; i < object->get_children_count(); i++) {
			_find_main_camera(s, object->get_child(i), pointer);
			if (pointer) return;
		}
	}
	gameobject* scene::find_main_camera() {
		gameobject* object = NULL;
		for (auto& obj : this->m_children) {
			_find_main_camera(this, obj.get(), object);
			if (object) break;
		}
		return object;
	}
	std::vector<light_component::light_data> scene::get_lights() {
		std::vector<light_component::light_data> data;
		std::vector<light_component*> lights = this->find_all_instances_of_component<light_component>();
		for (auto light : lights) {
			data.push_back(light->get_data());
		}
		return data;
	}
	scene::shadow_settings& scene::get_shadow_settings() {
		return this->m_shadow_settings;
	}
	void scene::set_shadow_defaults() {
		this->m_shadow_settings.near_plane = 1.f;
		this->m_shadow_settings.far_plane = 7.5f;
		this->m_shadow_settings.left = this->m_shadow_settings.bottom = -10.f;
		this->m_shadow_settings.right = this->m_shadow_settings.top = 10.f;
	}
	void scene::render_scene() {
		for (auto& c : this->m_children) {
			opengl_shader_library::shader::use(this->m_shader);
			c->render();
		}
	}
}