#include "pch.h"
#include "opengl_context.h"
#include "platform_misc.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_context::opengl_context(const window& w, opengl_data* data) : context(w) {
				auto temp = platform_specific::make_opengl_context(this->m_window.m, data->major_version, data->minor_version);
				this->m_context = new platform_specific::gl_context_t(temp);
			}
			void opengl_context::swap_buffers() {
				platform_specific::swap_buffers(*this->m_context);
			}
			void opengl_context::bind() {
				platform_specific::bind(*this->m_context);
			}
			void opengl_context::unbind() {
				auto null = platform_specific::make_null_context(*this->m_context);
				platform_specific::bind(null);
			}
			opengl_context::~opengl_context() {
				platform_specific::destroy_context(*this->m_context);
				delete this->m_context;
			}
		}
	}
}