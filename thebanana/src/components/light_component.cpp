#include "pch.h"
#include "components/light_component.h"
namespace thebanana {
	light_component::light_component(gameobject* object) : component(object) {
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Diffuse"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Specular"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Ambient"));
		this->add_property(new property<float>(0.1f, "Ambient strength"));
		graphics::framebuffer::specification spec;
		spec.width = spec.height = 1024;
		spec.buffers = spec.depth;
		graphics::framebuffer::attachment_settings& settings = spec.depth_settings;
		settings.type = GL_TEXTURE_CUBE_MAP;
		settings.wrap_r = GL_CLAMP_TO_EDGE;
		settings.attachment_type = GL_DEPTH_ATTACHMENT;
		settings.internal_format = GL_DEPTH_COMPONENT;
		settings.format = GL_DEPTH_COMPONENT;
		settings.texture_proc = [](graphics::framebuffer::specification* spec) {
			for (unsigned int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, spec->depth_settings.internal_format, spec->width, spec->height, 0, spec->depth_settings.format, GL_FLOAT, NULL);
			}
		};
		settings.framebuffer_attachment_proc = [](unsigned int id, graphics::framebuffer::specification* spec) {
			glFramebufferTexture(GL_FRAMEBUFFER, spec->depth_settings.attachment_type, id, 0);
		};
		this->m_depthbuffer = std::unique_ptr<graphics::framebuffer>(graphics::framebuffer::create(&spec));
	}
	light_component::light_data light_component::get_data() {
		light_data data;
		data.position = this->get_transform();
		data.diffuse = this->get_property<property_classes::color>("Diffuse")->get_vector();
		data.specular = this->get_property<property_classes::color>("Specular")->get_vector();
		data.ambient = this->get_property<property_classes::color>("Ambient")->get_vector();
		data.ambient_strength = *this->get_property<float>("Ambient strength");
		data.shadowmap = this->m_depthbuffer->get_attachments()[this->m_depthbuffer->get_attachment_map().depth_index].value;
		data.shadow_transforms = this->m_shadow_transforms;
		return data;
	}
	BANANA_API graphics::framebuffer* light_component::get_depthbuffer() {
		return this->m_depthbuffer.get();
	}
	void light_component::set_shadow_transforms(const std::vector<glm::mat4>& transforms) {
		this->m_shadow_transforms = transforms;
	}
}