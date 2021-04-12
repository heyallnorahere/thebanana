#include "pch.h"
#include "opengl_util.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			unsigned int get_target(unsigned int texture) {
				int target;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
				glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_TARGET, &target);
				glBindTexture(GL_TEXTURE_2D, NULL);
				return (unsigned int)target;
			}
			bool is_2d(unsigned int texture) {
				return get_target(texture) == GL_TEXTURE_2D;
			}
			unsigned int get_bound_framebuffer() {
				int bound_framebuffer;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_framebuffer);
				return (unsigned int)bound_framebuffer;
			}
			unsigned int get_current_shader() {
				int current_shader;
				glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);
				return (unsigned int)current_shader;
			}
			void unbind_all_textures(const std::vector<unsigned int>& types) {
				for (unsigned int texture_index = 0; texture_index < 30; texture_index++) {
					glActiveTexture(GL_TEXTURE0 + texture_index);
					for (auto type : types) {
						glBindTexture(type, NULL);
					}
				}
			}
			void get_texture_data(void* texid, void* buffer) {
				int format;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (size_t)texid);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, NULL, GL_TEXTURE_INTERNAL_FORMAT, &format);
				// all textures use GL_UNSIGNED_BYTE
				glGetTexImage(GL_TEXTURE_2D, NULL, format, GL_UNSIGNED_BYTE, buffer);
				glBindTexture(GL_TEXTURE_2D, NULL);
			}
			void get_texture_size(void* texid, size_t& width, size_t& height) {
				int w, h;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (size_t)texid);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, NULL, GL_TEXTURE_WIDTH, &w);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, NULL, GL_TEXTURE_HEIGHT, &h);
				glBindTexture(GL_TEXTURE_2D, NULL);
				width = w;
				height = h;
			}
		}
	}
}