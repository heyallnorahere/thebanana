#include "pch.h"
#include "opengl_framebuffer.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_framebuffer::opengl_framebuffer(specification* s) : framebuffer(s) {
				this->m_specification = *s;
				this->setup();
			}
			void opengl_framebuffer::bind() {
				glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
			}
			void opengl_framebuffer::unbind() {
				glBindFramebuffer(GL_FRAMEBUFFER, NULL);
			}
			void opengl_framebuffer::set_size(int width, int height) {
				this->m_specification.width = width;
				this->m_specification.height = height;
				this->reload();
			}
			void opengl_framebuffer::reload() {
				this->clean_up();
				this->setup();
			}
			void* opengl_framebuffer::get_id() {
				return &this->m_id;
			}
			std::vector<framebuffer::attachment> opengl_framebuffer::get_attachments() {
				return this->m_attachments;
			}
			opengl_framebuffer::~opengl_framebuffer() {
				this->clean_up();
			}
			void opengl_framebuffer::setup() {
				this->m_attachments.clear();
				glGenFramebuffers(1, &this->m_id);
				glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
				if (this->m_specification.buffers & specification::color) {
					attachment_settings settings = this->m_specification.color_settings;
					unsigned int type = (settings.type ? settings.type : GL_TEXTURE_2D);
					unsigned int internal_format = (settings.internal_format ? settings.internal_format : GL_RGBA8);
					unsigned int format = (settings.format ? settings.format : GL_RGBA);
					unsigned int attachment_type = (settings.attachment_type ? settings.attachment_type : GL_COLOR_ATTACHMENT0);
					unsigned int min_filter = (settings.min_filter ? settings.min_filter : GL_LINEAR);
					unsigned int mag_filter = (settings.mag_filter ? settings.mag_filter : GL_LINEAR);
					unsigned int color;
					glGenTextures(1, &color);
					glBindTexture(type, color);
					if (settings.texture_proc) settings.texture_proc(&this->m_specification);
					else glTexImage2D(type, NULL, internal_format, this->m_specification.width, this->m_specification.height, NULL, format, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min_filter);
					glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);
					if (settings.framebuffer_attachment_proc) settings.framebuffer_attachment_proc(color, &this->m_specification);
					else glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type + this->m_attachments.size(), type, color, NULL);
					this->m_attachment_map.color_index = this->m_attachments.size();
					this->m_attachments.push_back({ attachment::color, (void*)color });
				}
				if (this->m_specification.buffers & specification::depth) {
					attachment_settings settings = this->m_specification.depth_settings;
					unsigned int type = (settings.type ? settings.type : GL_TEXTURE_2D);
					unsigned int internal_format = (settings.internal_format ? settings.internal_format : GL_DEPTH24_STENCIL8);
					unsigned int format = (settings.format ? settings.format : GL_DEPTH24_STENCIL8);
					unsigned int attachment_type = (settings.attachment_type ? settings.attachment_type : GL_DEPTH_STENCIL_ATTACHMENT);
					unsigned int min_filter = (settings.min_filter ? settings.min_filter : GL_NEAREST);
					unsigned int mag_filter = (settings.mag_filter ? settings.mag_filter : GL_NEAREST);
					unsigned int wrap_s = (settings.wrap_s ? settings.wrap_s : GL_CLAMP_TO_BORDER);
					unsigned int wrap_t = (settings.wrap_t ? settings.wrap_t : GL_CLAMP_TO_BORDER);
					unsigned int depth;
					glGenTextures(1, &depth);
					glBindTexture(type, depth);
					if (settings.texture_proc) settings.texture_proc(&this->m_specification);
					else glTexImage2D(type, NULL, internal_format, this->m_specification.width, this->m_specification.height, NULL, format, GL_FLOAT, NULL);
					glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min_filter);
					glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);
					glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap_s);
					glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap_t);
					// by default, there is no WRAP_R property
					if (settings.wrap_r) {
						glTexParameteri(type, GL_TEXTURE_WRAP_R, settings.wrap_r);
					}
					if (settings.framebuffer_attachment_proc) settings.framebuffer_attachment_proc(depth, &this->m_specification);
					else glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, type, depth, 0);
					this->m_attachment_map.depth_index = this->m_attachments.size();
					this->m_attachments.push_back({ attachment::depth, (void*)depth });
				}
				glBindFramebuffer(GL_FRAMEBUFFER, NULL);
			}
			void opengl_framebuffer::clean_up() {
				if (this->m_specification.buffers & specification::color) {
					unsigned int color = (unsigned int)(size_t)this->m_attachments[this->m_attachment_map.color_index].value;
					glDeleteTextures(1, &color);
				}
				if (this->m_specification.buffers & specification::depth) {
					unsigned int depth = (unsigned int)(size_t)this->m_attachments[this->m_attachment_map.depth_index].value;
					glDeleteTextures(1, &depth);
				}
				glDeleteFramebuffers(1, &this->m_id);
			}
		}
	}
}