#include "pch.h"
#include "material.h"
#include <stb_image.h>
#include "internal_util.h"
namespace thebanana {
	material::material() {
		this->m_uuid = generate_uuid();
		unsigned char color[3];
		for (size_t i = 0; i < 3; i++) {
			color[i] = 0xff;
		}
		this->set_albedo(color, 1, 1, 3);
		this->m_color = glm::vec3(1.f);
	}
	void material::set_albedo(const std::string& image_path) {
		int width, height, channels;
		unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &channels, NULL);
		if (data) {
			this->m_albedo_path = image_path;
			this->set_albedo(data, width, height, channels, false);
			stbi_image_free(data);
		}
	}
	void material::set_albedo(void* data, int width, int height, int channels, bool clear_path) {
		if (clear_path) {
			this->m_albedo_path.clear();
		}
		graphics::texture::data d;
		d.pixels = data;
		d.width = width;
		d.height = height;
		d.channels = channels;
		this->m_albedo = std::shared_ptr<graphics::texture>(graphics::texture::create(d));
	}
	void material::set_albedo(graphics::texture* texture) {
		this->m_albedo = std::shared_ptr<graphics::texture>(texture);
	}
	const std::shared_ptr<graphics::texture>& material::get_albedo() {
		return this->m_albedo;
	}
	std::string material::get_albedo_path() {
		return this->m_albedo_path;
	}
	void material::set_color(glm::vec3 color) {
		this->m_color = color;
	}
	glm::vec3 material::get_color() {
		return this->m_color;
	}
	void material::set_shininess(float shininess) {
		this->m_shininess = shininess;
	}
	float material::get_shininess() {
		return this->m_shininess;
	}
	void material::send_to_shader(unsigned int shader_id, const std::string& uniform_name) {
		this->m_albedo->bind(10);
		auto get_uniform_name = [&](const std::string& name) {
			return uniform_name + "." + name;
		};
		opengl_shader_library::uni u(shader_id);
		u._int(get_uniform_name("albedo"), 10);
		//glUniform1i(glGetUniformLocation(shader_id, get_uniform_name("albedo").c_str()), 0);
		u.vec3(get_uniform_name("color"), this->m_color);
		//glUniform3fv(glGetUniformLocation(shader_id, get_uniform_name("color").c_str()), 1, glm::value_ptr(this->m_color));
		u._float(get_uniform_name("shininess"), this->m_shininess);
		//glUniform1f(glGetUniformLocation(shader_id, get_uniform_name("shininess").c_str()), this->m_shininess);
	}
	void material::set_uuid(unsigned long long uuid) {
		this->m_uuid = uuid;
	}
	unsigned long long material::get_uuid() {
		return this->m_uuid;
	}
	unsigned long long material_registry::new_material()  {
		material* m = new material;
		this->m_materials.push_back(std::unique_ptr<material>(m));
		return m->get_uuid();
	}
	material* material_registry::find(unsigned long long uuid) {
		for (auto& mat : this->m_materials) {
			if (mat->get_uuid() == uuid) {
				return mat.get();
			}
		}
		return NULL;
	}
	void material_registry::clear() {
		this->m_materials.clear();
	}
	size_t material_registry::get_count() {
		return this->m_materials.size();
	}
	material* material_registry::get(size_t index) {
		auto& mat = this->m_materials[index];
		return mat.get();
	}
}