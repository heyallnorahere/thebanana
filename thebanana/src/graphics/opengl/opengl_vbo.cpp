#include "pch.h"
#include "opengl_vbo.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_vbo::opengl_vbo(const data& d) : vertex_buffer(d) {
				this->setup();
			}
			void opengl_vbo::bind() {
				glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
			}
			void opengl_vbo::unbind() {
				glBindBuffer(GL_ARRAY_BUFFER, NULL);
			}
			void opengl_vbo::draw() {
				this->bind();
				glDrawArrays(GL_TRIANGLES, 0, this->m_data.vertex_count);
			}
			opengl_vbo::~opengl_vbo() {
				glDeleteBuffers(1, &this->m_id);
			}
			void opengl_vbo::setup() {
				glGenBuffers(1, &this->m_id);
				glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
				glBufferData(GL_ARRAY_BUFFER, this->m_data.size, this->m_data.data, GL_STATIC_DRAW);
				size_t total_size = 0;
				for (const auto& attr : this->m_data.attributes) {
					total_size += attr.size;
				}
				for (size_t i = 0; i < this->m_data.attributes.size(); i++) {
					size_t offset = 0;
					for (size_t j = 0; j < i; j++) {
						offset += this->m_data.attributes[j].size;
					}
					const auto& attr = this->m_data.attributes[i];
					unsigned int type = 0;
					switch (attr.type) {
					case data::vertex_attrib::type_float:
						type = GL_FLOAT;
						break;
					case data::vertex_attrib::type_uint:
						type = GL_UNSIGNED_INT;
						break;
					case data::vertex_attrib::type_int:
						type = GL_INT;
						break;
					case data::vertex_attrib::type_ubyte:
						type = GL_UNSIGNED_BYTE;
						break;
					case data::vertex_attrib::type_byte:
						type = GL_BYTE;
						break;
					}
					assert(type);
					size_t index = ((attr.custom_index < 0) ? i : (size_t)attr.custom_index);
					glEnableVertexAttribArray(index);
					switch (attr.type) {
					case data::vertex_attrib::type_uint:
					case data::vertex_attrib::type_int:
						glVertexAttribIPointer(index, attr.elements, type, total_size, (void*)offset);
						break;
					default:
						glVertexAttribPointer(index, attr.elements, type, attr.normalize, total_size, (void*)offset);
						break;
					}
				}
			}
		}
	}
}
