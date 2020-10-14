#include "pch.h"
#include "graphics/opengl/opengl_framebuffer.h"
namespace thebanana {
	opengl_framebuffer::opengl_framebuffer(desc* d) : framebuffer(d) {
		this->m_description = *d;
		this->setup();
	}
	void opengl_framebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
	}
	void opengl_framebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}
	void opengl_framebuffer::set_size(int width, int height) {
		this->m_description.width = width;
		this->m_description.height = height;
		this->reload();
	}
	void opengl_framebuffer::reload() {
		this->clean_up();
		this->setup();
	}
	std::vector<framebuffer::attachment> opengl_framebuffer::get_attachments() {
		return this->m_attachments;
	}
	opengl_framebuffer::~opengl_framebuffer() {
		this->clean_up();
	}
	void opengl_framebuffer::setup() {
		this->m_attachments.clear();
		glCreateFramebuffers(1, &this->m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
		if (this->m_description.buffers & desc::color) {
			unsigned int color;
			glCreateTextures(GL_TEXTURE_2D, 1, &color);
			glBindTexture(GL_TEXTURE_2D, color);
			glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGBA8, this->m_description.width, this->m_description.height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->m_attachments.size(), GL_TEXTURE_2D, color, NULL);
			this->m_attachment_map.color_index = this->m_attachments.size();
			this->m_attachments.push_back({ attachment::color, (void*)color });
		}
		if (this->m_description.buffers & desc::depth) {
			unsigned int depth;
			glCreateTextures(GL_TEXTURE_2D, 1, &depth);
			glBindTexture(GL_TEXTURE_2D, depth);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->m_description.width, this->m_description.height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
			this->m_attachment_map.depth_index = this->m_attachments.size();
			this->m_attachments.push_back({ attachment::depth, (void*)depth });
		}
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}
	void opengl_framebuffer::clean_up() {
		if (this->m_description.buffers & desc::color) {
			unsigned int color = (unsigned int)this->m_attachments[this->m_attachment_map.color_index].value;
			glDeleteTextures(1, &color);
		}
		if (this->m_description.buffers & desc::depth) {
			unsigned int depth = (unsigned int)this->m_attachments[this->m_attachment_map.depth_index].value;
			glDeleteTextures(1, &depth);
		}
		glDeleteFramebuffers(1, &this->m_id);
	}
}