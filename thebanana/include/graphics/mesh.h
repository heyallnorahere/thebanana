#pragma once
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class vertex_array_object;
		class vertex_buffer;
		class index_buffer;
		class mesh {
		public:
			struct vertex {
				glm::vec3 position, normal;
				glm::vec2 uv;
			};
			BANANA_API mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices);
			BANANA_API void render(int animation_index);
			BANANA_API ~mesh();
		private:
			void setup();
			vertex_array_object* m_vao;
			vertex_buffer* m_vbo;
			index_buffer* m_ebo;
			std::vector<vertex> m_vertices;
			std::vector<unsigned int> m_indices;
		};
	}
}