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
		BANANA_API ~model();
	private:
		void load(const std::string& path);
		void process_node(aiNode* node);
		void process_mesh(aiMesh* mesh);
		Assimp::Importer* m_importer;
		const aiScene* m_scene;
		std::vector<std::unique_ptr<graphics::mesh>> m_meshes;
		bool m_loaded;
	};
}