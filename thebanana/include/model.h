#pragma once
#include "banana_api.h"
#include "graphics/mesh.h"
namespace Assimp {
	class Importer;
}
struct aiScene;
struct aiNode;
struct aiMesh;
namespace thebanana {
	class model {
	public:
		BANANA_API model(const std::string& path);
		BANANA_API bool is_loaded();
		BANANA_API const aiScene* get_scene();
		BANANA_API void draw(float time, int animation_id = -1);
		BANANA_API ~model();
	private:
		struct bone_info {
			glm::mat4 bone_offset, final_transform;
			bone_info() {
				memset(&this->bone_offset, 0, sizeof(glm::mat4));
				memset(&this->final_transform, 0, sizeof(glm::mat4));
			}
		};
		void load(const std::string& path);
		void process_node(aiNode* node);
		void process_mesh(aiMesh* mesh);
		void read_node_hierarchy(float time, const aiNode* node, const glm::mat4& parent_transform, int animation_id);
		Assimp::Importer* m_importer;
		const aiScene* m_scene;
		std::vector<std::unique_ptr<graphics::mesh>> m_meshes;
		bool m_loaded;
		glm::mat4 m_global_inverse_transform;
		std::map<std::string, size_t> m_bone_mapping;
		std::vector<bone_info> m_bone_info;
		size_t m_bone_count;
	};
}