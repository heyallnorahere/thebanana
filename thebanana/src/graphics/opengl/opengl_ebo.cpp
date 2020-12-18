#include "pch.h"
#include "opengl_ebo.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_ebo::opengl_ebo(const data& d) : index_buffer(d) {
				this->setup();
			}
			void opengl_ebo::bind() {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
			}
			void opengl_ebo::unbind() {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
			}
			void opengl_ebo::draw() {
				unsigned int type = 0;
				switch (this->m_data.type) {
				case data::type_uint:
					type = GL_UNSIGNED_INT;
					break;
				case data::type_int:
					type = GL_INT;
					break;
				case data::type_ubyte:
					type = GL_UNSIGNED_BYTE;
					break;
				case data::type_byte:
					type = GL_BYTE;
					break;
				}
				if (!type) return;
				this->bind();
				glDrawElements(GL_TRIANGLES, this->m_data.index_count, type, NULL);
			}
			opengl_ebo::~opengl_ebo() {
				glDeleteBuffers(1, &this->m_id);
			}
			void opengl_ebo::setup() {
				glGenBuffers(1, &this->m_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_data.size, this->m_data.data, GL_STATIC_DRAW);
			}
		}
	}
}