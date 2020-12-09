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
	}
	void material::set_albedo(const std::string& image_path) {
		int width, height, channels;
		unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &channels, NULL);
		if (data) {
			this->set_albedo(data, width, height, channels);
			stbi_image_free(data);
		}
	}
	void material::set_albedo(void* data, int width, int height, int channels) {
		graphics::texture::data d;
		d.pixels = data;
		d.width = width;
		d.height = height;
		d.channels = channels;
		this->m_albedo = std::unique_ptr<graphics::texture>(graphics::texture::create(d));
	}
	void material::set_albedo(graphics::texture* texture) {
		this->m_albedo = std::unique_ptr<graphics::texture>(texture);
	}
	graphics::texture* material::get_albedo() {
		return this->m_albedo.get();
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_albedo->get_id());
		auto get_uniform_name = [&](const std::string& name) {
			return uniform_name + "." + name;
		};
		glUniform1i(glGetUniformLocation(shader_id, get_uniform_name("albedo").c_str()), 0);
		glUniform3fv(glGetUniformLocation(shader_id, get_uniform_name("color").c_str()), 1, glm::value_ptr(this->m_color));
		glUniform1f(glGetUniformLocation(shader_id, get_uniform_name("shininess").c_str()), this->m_shininess);
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
		this->m_materials;
	}
	size_t material_registry::get_count() {
		return this->m_materials.size();
	}
	material* material_registry::get(size_t index) {
		auto& mat = this->m_materials[index];
		return mat.get();
	}
}