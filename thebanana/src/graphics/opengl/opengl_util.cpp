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
			graphics_enum to_enum(unsigned int e) {
				switch (e) {
				case GL_FRAMEBUFFER:
					return FRAMEBUFFER;
					break;
				case GL_TEXTURE_2D:
					return TEXTURE_2D;
					break;
				case GL_TEXTURE_CUBE_MAP:
					return TEXTURE_CUBE_MAP;
					break;
				case GL_RGBA8:
					return RGBA8;
					break;
				case GL_DEPTH24_STENCIL8:
					return DEPTH24_STENCIL8;
					break;
				case GL_RGBA:
					return RGBA;
					break;
				case GL_DEPTH_STENCIL:
					return DEPTH_STENCIL;
					break;
				case GL_DEPTH_COMPONENT:
					return DEPTH_COMPONENT;
					break;
				case GL_COLOR_ATTACHMENT0:
					return COLOR_ATTACHMENT;
					break;
				case GL_DEPTH_ATTACHMENT:
					return DEPTH_ATTACHMENT;
					break;
				case GL_DEPTH_STENCIL_ATTACHMENT:
					return DEPTH_STENCIL_ATTACHMENT;
					break;
				case GL_LINEAR:
					return LINEAR;
					break;
				case GL_NEAREST:
					return NEAREST;
					break;
				case GL_UNSIGNED_BYTE:
					return UNSIGNED_BYTE;
					break;
				case GL_UNSIGNED_INT:
					return UNSIGNED_INT;
					break;
				case GL_FLOAT:
					return FLOAT;
					break;
				case GL_INT:
					return INT;
					break;
				case GL_BYTE:
					return BYTE;
					break;
				case GL_CLAMP_TO_BORDER:
					return CLAMP_TO_BORDER;
					break;
				case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
					return TEXTURE_CUBE_MAP_POSITIVE_X;
					break;
				default:
					return NONE;
					break;
				}
			}
			unsigned int from_enum(graphics_enum e) {
				switch (e) {
				case FRAMEBUFFER:
					return GL_FRAMEBUFFER;
					break;
				case TEXTURE_2D:
					return GL_TEXTURE_2D;
					break;
				case TEXTURE_CUBE_MAP:
					return GL_TEXTURE_CUBE_MAP;
					break;
				case RGBA8:
					return GL_RGBA8;
					break;
				case DEPTH24_STENCIL8:
					return GL_DEPTH24_STENCIL8;
					break;
				case RGBA:
					return GL_RGBA;
					break;
				case DEPTH_STENCIL:
					return GL_DEPTH_STENCIL;
					break;
				case DEPTH_COMPONENT:
					return GL_DEPTH_COMPONENT;
					break;
				case COLOR_ATTACHMENT:
					return GL_COLOR_ATTACHMENT0;
					break;
				case DEPTH_ATTACHMENT:
					return GL_DEPTH_ATTACHMENT;
					break;
				case DEPTH_STENCIL_ATTACHMENT:
					return GL_DEPTH_STENCIL_ATTACHMENT;
					break;
				case LINEAR:
					return GL_LINEAR;
					break;
				case NEAREST:
					return GL_NEAREST;
					break;
				case UNSIGNED_BYTE:
					return GL_UNSIGNED_BYTE;
					break;
				case UNSIGNED_INT:
					return GL_UNSIGNED_INT;
					break;
				case FLOAT:
					return GL_FLOAT;
					break;
				case INT:
					return GL_INT;
					break;
				case BYTE:
					return GL_BYTE;
					break;
				case CLAMP_TO_BORDER:
					return GL_CLAMP_TO_BORDER;
					break;
				case TEXTURE_CUBE_MAP_POSITIVE_X:
					return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
					break;
				default:
					return 0;
					break;
				}
			}
		}
	}
}