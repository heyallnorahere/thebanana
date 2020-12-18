#include "pch.h"
#include "graphics/mesh.h"
#include "graphics/vertex_array_object.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
namespace thebanana {
	namespace graphics {
		mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices) {
			this->m_vertices = vertices;
			this->m_indices = indices;
			this->setup();
		}
		void mesh::render() {
			this->m_vao->bind();
			this->m_ebo->draw();
			this->m_vao->unbind();
		}
		mesh::~mesh() {
			delete this->m_ebo;
			delete this->m_vbo;
			delete this->m_vao;
		}
		void mesh::setup() {
			this->m_vao = vertex_array_object::create();
			this->m_vao->bind();
			vertex_buffer::data vertex_buffer_data;
			vertex_buffer_data.data = this->m_vertices.data();
			vertex_buffer_data.size = this->m_vertices.size() * sizeof(vertex);
			vertex_buffer_data.vertex_count = this->m_vertices.size();
			vertex_buffer::data::vertex_attrib attrib;
			attrib.size = sizeof(glm::vec3);
			attrib.type = vertex_buffer::data::vertex_attrib::type_float;
			attrib.normalize = false;
			attrib.elements = 3;
			vertex_buffer_data.attributes.push_back(attrib);
			vertex_buffer_data.attributes.push_back(attrib);
			attrib.size = sizeof(glm::vec2);
			attrib.elements = 2;
			vertex_buffer_data.attributes.push_back(attrib);
			this->m_vbo = vertex_buffer::create(vertex_buffer_data);
			index_buffer::data index_buffer_data;
			index_buffer_data.data = this->m_indices.data();
			index_buffer_data.size = this->m_indices.size() * sizeof(unsigned int);
			index_buffer_data.index_count = this->m_indices.size();
			index_buffer_data.type = index_buffer_data.type_uint;
			this->m_ebo = index_buffer::create(index_buffer_data);
			this->m_vao->unbind();
		}
	}
}