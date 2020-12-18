#include "pch.h"
#include "opengl_vao.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_vao::opengl_vao() {
				glGenVertexArrays(1, &this->m_id);
			}
			void opengl_vao::bind() {
				glBindVertexArray(this->m_id);
			}
			void opengl_vao::unbind() {
				glBindVertexArray(NULL);
			}
			opengl_vao::~opengl_vao() {
				glDeleteVertexArrays(1, &this->m_id);
			}
		}
	}
}