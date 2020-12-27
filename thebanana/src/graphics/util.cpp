#include "pch.h"
#include "graphics/util.h"
namespace thebanana {
	namespace graphics {
		namespace util {
			unsigned int get_target(unsigned int texture) {
				int target;
				glGetTextureParameteriv(texture, GL_TEXTURE_TARGET, &target);
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
		}
	}
}