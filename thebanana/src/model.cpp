#include "pch.h"
#include "model.h"
#include "game.h"
#include "graphics/graphics_types.h"
namespace thebanana {
	model::model(const std::string& path) {
		this->m_loaded = false;
		this->load(path);
	}
	bool model::is_loaded() {
		return this->m_loaded;
	}
	model::~model() {
		delete this->m_importer;
	}
	void model::load(const std::string& path) {
		this->m_importer = new Assimp::Importer;
		unsigned int flags = aiProcess_Triangulate | aiProcess_LimitBoneWeights;
		if (graphics::get_default_graphics_api() == graphics::graphics_api::opengl) {
			flags |= aiProcess_FlipUVs;
		}
		this->m_scene = this->m_importer->ReadFile(path, flags);
		if (!this->m_scene || this->m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !this->m_scene->mRootNode) {
			g_game->debug_print("assimp error: " + std::string(this->m_importer->GetErrorString()));
			return;
		}
		this->process_node(this->m_scene->mRootNode);
		this->m_loaded = true;
	}
	void model::process_node(aiNode* node) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = this->m_scene->mMeshes[node->mMeshes[i]];
			this->process_mesh(mesh);
		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			this->process_node(node->mChildren[i]);
		}
	}
	void model::process_mesh(aiMesh* mesh) {
		std::vector<graphics::mesh::vertex> vertices;
		std::vector<unsigned int> indices;
		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			graphics::mesh::vertex vertex;
			aiVector3D pos = mesh->mVertices[i];
			vertex.position = glm::vec3(pos.x, pos.y, pos.z);
			aiVector3D normal = mesh->mNormals[i];
			vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
			if (mesh->mTextureCoords[0]) {
				aiVector3D uv = mesh->mTextureCoords[0][i];
				vertex.uv = glm::vec2(uv.x, uv.y);
			}
			else {
				vertex.uv = glm::vec2(0.f);
			}
			vertices.push_back(vertex);
		}
		for (size_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[i]);
			}
		}
		this->m_meshes.push_back(std::unique_ptr<graphics::mesh>(new graphics::mesh(vertices, indices)));
	}
}