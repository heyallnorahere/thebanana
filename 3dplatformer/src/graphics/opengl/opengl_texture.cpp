#include "pch.h"
#include "graphics/opengl/opengl_texture.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_texture::opengl_texture(const data& d, void* api_specific) : texture(d) {
				if (api_specific) {
					this->m_settings = *(settings*)api_specific;
				}
				this->create_texture(d.pixels);
			}
			void opengl_texture::bind() {
				glBindTexture(GL_TEXTURE_2D, this->id);
			}
			void opengl_texture::unbind() {
				glBindTexture(GL_TEXTURE_2D, NULL);
			}
			void opengl_texture::set_data(void* pixels) {
				this->bind();
				int format;
				switch (this->m_channels) {
				case 1:
					format = GL_RED;
					break;
				case 2:
					format = GL_RG;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
				default:
					return;
					break;
				}
				glTexImage2D(GL_TEXTURE_2D, NULL, format, this->m_width, this->m_height, NULL, format, GL_UNSIGNED_BYTE, pixels);
				if (pixels) {
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				this->unbind();
			}
			opengl_texture::~opengl_texture() {
				this->destroy_texture();
			}
			void opengl_texture::create_texture(void* pixels) {
				glGenTextures(1, &this->id);
				this->bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_settings.wrap_s ? this->m_settings.wrap_s : GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_settings.wrap_t ? this->m_settings.wrap_t : GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_settings.min_filter ? this->m_settings.min_filter : GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_settings.mag_filter ? this->m_settings.mag_filter : GL_LINEAR);
				this->set_data(pixels);
			}
			void opengl_texture::destroy_texture() {
				glDeleteTextures(1, &this->id);
			}
		}
	}
}