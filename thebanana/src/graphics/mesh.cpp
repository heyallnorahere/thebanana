#include "pch.h"
#include "graphics/mesh.h"
namespace thebanana {
	namespace graphics {
		mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices) {
			this->m_vertices = vertices;
			this->m_indices = indices;
			this->setup();
		}
		void mesh::render(int animation_index) {
			// todo: implement
		}
		mesh::~mesh() {
			// todo: implement
		}
		void mesh::setup() {
			// todo: implement
		}
	}
}