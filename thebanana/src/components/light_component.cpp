#include "pch.h"
#include "components/light_component.h"
namespace thebanana {
	light_component::light_component(gameobject* object) : component(object) {
		this->add_property(new property<bool>(true, "Point"));
		this->add_property(new property<bool>(false, "Directional"));
		this->add_property(new property<glm::vec3>(glm::vec3(0.f, -1.f, 0.f), "Direction"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Diffuse"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Specular"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Ambient"));
		this->add_property(new property<float>(0.1f, "Ambient strength"));
		this->m_type = light_type::none;
	}
	void light_component::update() {
		this->update_depthbuffer();
	}
	light_component::light_data light_component::get_data() {
		light_data data;
		data.type = this->m_type;
		data.position = this->get_transform();
		data.direction = *this->get_property<glm::vec3>("Direction");
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
	void light_component::update_depthbuffer() {
		light_type type = this->get_type();
		if (this->m_type != type) {
			this->m_type = type;
			graphics::framebuffer::specification spec;
			spec.width = spec.height = 1024;
			spec.buffers = spec.depth;
			graphics::framebuffer::attachment_settings& depth_settings = spec.depth_settings;
			switch (this->m_type) {
			case light_type::point:
				depth_settings.type = GL_TEXTURE_CUBE_MAP;
				depth_settings.wrap_r = GL_CLAMP_TO_EDGE;
				depth_settings.attachment_type = GL_DEPTH_ATTACHMENT;
				depth_settings.internal_format = GL_DEPTH_COMPONENT;
				depth_settings.format = GL_DEPTH_COMPONENT;
				depth_settings.texture_proc = [](graphics::framebuffer::specification* spec) {
					for (unsigned int i = 0; i < 6; i++) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, spec->depth_settings.internal_format, spec->width, spec->height, 0, spec->depth_settings.format, GL_FLOAT, NULL);
					}
				};
				depth_settings.framebuffer_attachment_proc = [](unsigned int id, graphics::framebuffer::specification* spec) {
					glFramebufferTexture(GL_FRAMEBUFFER, spec->depth_settings.attachment_type, id, 0);
				};
				break;
			case light_type::directional:
			case light_type::spotlight:
				depth_settings.type = GL_TEXTURE_2D;
				depth_settings.attachment_type = GL_DEPTH_ATTACHMENT;
				depth_settings.internal_format = GL_DEPTH_COMPONENT;
				depth_settings.format = GL_DEPTH_COMPONENT;
				depth_settings.texture_proc = [](graphics::framebuffer::specification* spec) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, spec->width, spec->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				};
				break;
			default:
				this->m_depthbuffer = std::unique_ptr<graphics::framebuffer>();
				return;
			}
			this->m_depthbuffer = std::unique_ptr<graphics::framebuffer>(graphics::framebuffer::create(&spec));
		}
	}
	light_component::light_type light_component::get_type() {
		bool point = *this->get_property<bool>("Point");
		bool directional = *this->get_property<bool>("Directional");
		unsigned int flags = 0;
		if (point) flags |= (unsigned int)light_type::point;
		if (directional) flags |= (unsigned int)light_type::directional;
		return (light_type)flags;
	}
}