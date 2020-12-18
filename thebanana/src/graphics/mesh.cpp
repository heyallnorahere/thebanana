#include "pch.h"
#include "graphics/mesh.h"
#include "graphics/vertex_array_object.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics/util.h"
namespace thebanana {
	namespace graphics {
		mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<vertex_bone_data>& bone_data) {
			this->m_vertices = vertices;
			this->m_indices = indices;
			this->m_bone_data = bone_data;
			this->setup();
		}
		void mesh::render() {
			unsigned int shader = util::get_current_shader();
			// todo: replace opengl calls with engine calls once a shader class is implemented
			if (shader) glUniform1i(glGetUniformLocation(shader, "has_bones"), !this->m_bone_data.empty());
			this->m_vao->bind();
			this->m_ebo->draw();
			this->m_vao->unbind();
		}
		mesh::~mesh() {
			delete this->m_bones;
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
			vertex_buffer_data.data = this->m_bone_data.data();
			vertex_buffer_data.size = this->m_bone_data.size() * sizeof(vertex_bone_data);
			vertex_buffer_data.vertex_count = this->m_bone_data.size();
			vertex_buffer_data.attributes.clear();
			attrib.size = sizeof(glm::vec<vertex_bone_data::max_bones_per_vertex, int>);
			attrib.type = vertex_buffer::data::vertex_attrib::type_int;
			attrib.normalize = false;
			attrib.elements = vertex_bone_data::max_bones_per_vertex;
			vertex_buffer_data.attributes.push_back(attrib);
			attrib.size = sizeof(glm::vec<vertex_bone_data::max_bones_per_vertex, float>);
			attrib.type = vertex_buffer::data::vertex_attrib::type_float;
			vertex_buffer_data.attributes.push_back(attrib);
			this->m_bones = vertex_buffer::create(vertex_buffer_data);
			this->m_vao->unbind();
		}
		void mesh::vertex_bone_data::add_bone_data(unsigned int id, float weight) {
			for (size_t i = 0; i < max_bones_per_vertex; i++) {
				if (this->weights[i] < 0.001f) {
					this->ids[i] = id;
					this->weights[i] = weight;
					return;
				}
			}
			assert(false); // we dont have room for more bones
		}
		mesh::vertex_bone_data::vertex_bone_data() {
			memset(this->ids, 0, sizeof(unsigned int) * max_bones_per_vertex);
			memset(this->weights, 0, sizeof(float) * max_bones_per_vertex);
		}
	}
}