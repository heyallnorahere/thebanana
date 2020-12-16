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
#include "graphics/util.h"
namespace thebanana {
	scene::scene(game* g) {
		this->m_shader = NULL;
		this->m_game = g;
		this->set_shadow_defaults();
		this->m_generating_shadows = false;
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
		this->set_shadow_defaults();
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
		bool has_depth_shader = false;
		for (auto p : this->m_depth_shaders) {
			if (p.second) {
				has_depth_shader = true;
				break;
			}
		}
		if (has_depth_shader) {
			this->generate_shadows();
		}
		this->m_generating_shadows = false;
		glViewport(0, 0, (int)this->m_game->get_window_size().x, (int)this->m_game->get_window_size().y);
		opengl_shader_library::shader::use(this->m_shader);
		this->m_shader->get_uniforms()._float("far_plane", this->m_shadow_settings.far_plane);
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
	opengl_shader_library::shader* scene::get_current_shader() {
		return this->m_generating_shadows ? this->m_depth_shaders[this->m_current_light_type] : this->m_shader;
	}
	void scene::set_shader_name(const std::string& shader_name) {
		this->m_shader = this->m_game->get_shader_registry()->get_shader(shader_name);
	}
	void scene::set_depth_shader_name(light_component::light_type type, const std::string& shader_name) {
		this->m_depth_shaders[type] = this->m_game->get_shader_registry()->get_shader(shader_name);
	}
	bool scene::is_generating_shadows() const {
		return this->m_generating_shadows;
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
			c->render();
		}
		graphics::util::unbind_all_textures({ GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP });
	}
	void scene::generate_shadows() {
		this->m_generating_shadows = true;
		unsigned int bound_framebuffer = graphics::util::get_bound_framebuffer();
		std::vector<light_component*> lights = this->find_all_instances_of_component<light_component>();
		glm::mat4 shadow_projection = glm::perspective(glm::radians(90.f), 1.f, this->m_shadow_settings.near_plane, this->m_shadow_settings.far_plane);
		for (auto light : lights) {
			if (!light->get_depthbuffer()) light->update_depthbuffer();
			light->get_depthbuffer()->bind();
			glViewport(0, 0, 1024, 1024);
			this->m_game->clear_screen();
			auto light_data = light->get_data();
			this->m_current_light_type = light_data.type;
			if (!this->get_current_shader()) continue;
			opengl_shader_library::shader::use(this->get_current_shader());
			auto& u = this->get_current_shader()->get_uniforms();
			switch (this->m_current_light_type) {
			case light_component::light_type::point:
			{
				u.vec3("lightpos", light_data.position);
				u._float("far_plane", this->m_shadow_settings.far_plane);
				std::vector<glm::mat4> shadow_transforms;
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				shadow_transforms.push_back(shadow_projection * glm::lookAt(light_data.position, light_data.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				for (size_t i = 0; i < shadow_transforms.size(); i++) {
					u.mat4("shadow_matrices[" + std::to_string(i) + "]", shadow_transforms[i]);
				}
			}
				break;
			case light_component::light_type::directional:
			case light_component::light_type::spotlight:
			{
				glm::vec3 lightpos = light_data.position;
				if (this->m_current_light_type == light_component::light_type::directional) {
					lightpos -= light_data.direction;
				}
				glm::mat4 light_projection = glm::ortho(this->m_shadow_settings.left, this->m_shadow_settings.right,
					this->m_shadow_settings.bottom, this->m_shadow_settings.top,
					this->m_shadow_settings.near_plane, this->m_shadow_settings.far_plane);
				glm::mat4 light_view = glm::lookAt(lightpos, lightpos + light_data.direction, glm::vec3(0.f, 1.f, 0.f));
				glm::mat4 light_space_matrix = light_projection * light_view;
				light->set_light_space_matrix(light_space_matrix);
				u.mat4("light_space_matrix", light_space_matrix);
			}
				break;
			}
			this->render_scene();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);
	}
}