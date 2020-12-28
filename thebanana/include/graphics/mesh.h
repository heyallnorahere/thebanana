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
			struct vertex_bone_data {
				static constexpr size_t max_bones_per_vertex = 4;
				unsigned int ids[max_bones_per_vertex];
				float weights[max_bones_per_vertex];
				BANANA_API void add_bone_data(unsigned int id, float weight);
				BANANA_API vertex_bone_data();
			};
			BANANA_API mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<vertex_bone_data>& bone_data = std::vector<vertex_bone_data>(), bool has_bones = false);
			BANANA_API void render();
			BANANA_API ~mesh();
		private:
			void setup();
			vertex_array_object* m_vao;
			vertex_buffer* m_vbo;
			index_buffer* m_ebo;
			vertex_buffer* m_bones;
			std::vector<vertex> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<vertex_bone_data> m_bone_data;
			bool m_has_bones;
		};
	}
}