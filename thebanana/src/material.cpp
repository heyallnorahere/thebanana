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
		this->m_diffuse = this->m_specular = this->m_ambient = glm::vec3(1.f);
		this->m_friendly_name = "New material";
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
	void material::set_diffuse(glm::vec3 color) {
		this->m_diffuse = color;
	}
	void material::set_specular(glm::vec3 color) {
		this->m_specular = color;
	}
	void material::set_ambient(glm::vec3 color) {
		this->m_ambient = color;
	}
	glm::vec3 material::get_diffuse() {
		return this->m_diffuse;
	}
	glm::vec3 material::get_specular() {
		return this->m_specular;
	}
	glm::vec3 material::get_ambient() {
		return this->m_ambient;
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
		u.vec3(get_uniform_name("diffuse"), this->m_diffuse);
		u.vec3(get_uniform_name("specular"), this->m_specular);
		u.vec3(get_uniform_name("ambient"), this->m_ambient);
		u._float(get_uniform_name("shininess"), this->m_shininess);
	}
	void material::set_uuid(unsigned long long uuid) {
		this->m_uuid = uuid;
	}
	unsigned long long material::get_uuid() {
		return this->m_uuid;
	}
	std::string material::get_friendly_name() {
		return this->m_friendly_name;
	}
	void material::set_friendly_name(const std::string& name) {
		this->m_friendly_name = name;
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