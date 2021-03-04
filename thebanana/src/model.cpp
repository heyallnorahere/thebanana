#include "pch.h"
#include "model.h"
#include "game.h"
#include "transform.h"
#include "graphics/graphics_types.h"
#include "graphics/util.h"
#include "internal_util.h"
namespace thebanana {
	static glm::mat4 from_assimp(const aiMatrix4x4& m, bool transpose = false) {
		glm::mat4 result(
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4
		);

		if (transpose) result = glm::transpose(result);
		return result;
	}
	static glm::mat4 to_scaling_matrix(aiVector3D scaling) {
		glm::mat4 m(1.f);
		m[0][0] = scaling.x;    m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
		m[1][0] = 0.0f;   m[1][1] = scaling.y;    m[1][2] = 0.0f;   m[1][3] = 0.0f;
		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaling.z;    m[2][3] = 0.0f;
		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
		return m;
	}
	static glm::mat4 to_translation_matrix(aiVector3D position) {
		glm::mat4 m(1.f);
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = position.x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = position.y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = position.z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		return m;
	}
	model::model(const std::string& path) {
		this->m_loaded = false;
		this->m_bone_count = 0;
		this->load(path);
	}
	bool model::is_loaded() {
		return this->m_loaded;
	}
	const aiScene* model::get_scene() {
		return this->m_scene;
	}
	void model::draw(float time, int animation_id) {
		glm::mat4 identity(1.f);
		float animation_time = 0.f;
		if (animation_id != -1 && animation_id < this->m_scene->mNumAnimations) {
			assert(this->m_scene->HasAnimations());
			float ticks_per_second = (float)(this->m_scene->mAnimations[animation_id]->mTicksPerSecond != 0 ? this->m_scene->mAnimations[animation_id]->mTicksPerSecond : 25.0f);
			float time_in_ticks = time * ticks_per_second;
			animation_time = fmod(time_in_ticks, (float)this->m_scene->mAnimations[animation_id]->mDuration);
		}
		this->read_node_hierarchy(animation_time, this->m_scene->mRootNode, identity, animation_id);
		unsigned int shader = graphics::util::get_current_shader();
		if (shader) {
			for (size_t i = 0; i < this->m_bone_count; i++) {
				assert(i < 100);
				char name[256];
				sprintf(name, "bones[%d]", i);
				// todo: replace with engine call
				auto location = glGetUniformLocation(shader, name);
				glUniformMatrix4fv(location, 1, true, glm::value_ptr(this->m_bone_info[i].final_transform));
			}
		}
		for (auto& mesh : this->m_meshes) {
			mesh->render();
		}
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
		this->m_global_inverse_transform = glm::inverse(from_assimp(this->m_scene->mRootNode->mTransformation));
		this->process_node(this->m_scene->mRootNode);
		this->m_loaded = true;
	}
	void model::process_node(aiNode* node) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = this->m_scene->mMeshes[node->mMeshes[i]];
			this->process_mesh(mesh, node);
		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			this->process_node(node->mChildren[i]);
		}
	}
	void model::process_mesh(aiMesh* mesh, aiNode* parent) {
		std::vector<graphics::mesh::vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumVertices);
		std::vector<graphics::mesh::vertex_bone_data> bone_data;
		bone_data.resize(mesh->mNumVertices);
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
				indices.push_back(face.mIndices[j]);
			}
		}
		for (int i = 0; i < mesh->mNumBones; i++) {
			size_t bone_index = 0;
			aiBone*& bone = mesh->mBones[i];
			std::string bone_name(bone->mName.data);
			if (this->m_bone_mapping.find(bone_name) == this->m_bone_mapping.end()) {
				bone_index = this->m_bone_count;
				this->m_bone_count++;
				bone_info bi;
				bi.bone_offset = from_assimp(bone->mOffsetMatrix);
				this->m_bone_info.push_back(bi);
				this->m_bone_mapping[bone_name] = bone_index;
			}
			else {
				bone_index = this->m_bone_mapping[bone_name];
			}
			for (int j = 0; j < bone->mNumWeights; j++) {
				aiVertexWeight& w = bone->mWeights[j];
				int vertex_id = w.mVertexId;
				float weight = w.mWeight;
				bone_data[vertex_id].add_bone_data(bone_index, weight);
			}
		}
		this->m_meshes.push_back(std::unique_ptr<graphics::mesh>(new graphics::mesh(vertices, indices, bone_data, mesh->HasBones())));
		per_mesh_function ptr = get_per_mesh_function();
		if (ptr) ptr(mesh, parent, this, vertices, indices, bone_data, get_user_data());
	}
	void calc_interpolated_scaling(aiVector3D& out, float animation_time, const aiNodeAnim* node_animation);
	void calc_interpolated_rotation(aiQuaternion& out, float animation_time, const aiNodeAnim* node_animation);
	void calc_interpolated_position(aiVector3D& out, float animation_time, const aiNodeAnim* node_animation);
	const aiNodeAnim* find_node_animation(const aiAnimation* animation, const std::string& node_name);
	void model::read_node_hierarchy(float time, const aiNode* node, const glm::mat4& parent_transform, int animation_id) {
		std::string node_name = std::string(node->mName.data, node->mName.length);
		const aiAnimation* animation = NULL;
		if (animation_id != -1 && animation_id < this->m_scene->mNumAnimations) {
			animation = this->m_scene->mAnimations[animation_id];
		}
		glm::mat4 node_transform = from_assimp(node->mTransformation);
		if (animation_id != -1 && animation_id < this->m_scene->mNumAnimations) {
			const aiNodeAnim* node_animation = find_node_animation(animation, node_name);
			if (node_animation) {
				aiVector3D scaling;
				calc_interpolated_scaling(scaling, time, node_animation);
				glm::mat4 scaling_m = to_scaling_matrix(scaling);
				aiQuaternion rotation;
				calc_interpolated_rotation(rotation, time, node_animation);
				glm::mat4 rotation_m = from_assimp(aiMatrix4x4(rotation.GetMatrix()));
				aiVector3D position;
				calc_interpolated_position(position, time, node_animation);
				glm::mat4 position_m = to_translation_matrix(position);
				node_transform = scaling_m * rotation_m * position_m;
			}
		}
		glm::mat4 global_transform = node_transform * parent_transform;
		if (this->m_bone_mapping.find(node_name) != this->m_bone_mapping.end()) {
			size_t bone_index = this->m_bone_mapping[node_name];
			this->m_bone_info[bone_index].final_transform = this->m_bone_info[bone_index].bone_offset * global_transform * this->m_global_inverse_transform;
		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			this->read_node_hierarchy(time, node->mChildren[i], global_transform, animation_id);
		}
	}
	const aiNodeAnim* find_node_animation(const aiAnimation* animation, const std::string& node_name) {
		for (size_t i = 0; i < animation->mNumChannels; i++) {
			const aiNodeAnim* node_animation = animation->mChannels[i];
			std::string name = std::string(node_animation->mNodeName.data, node_animation->mNodeName.length);
			if (name == node_name) return node_animation;
		}
		return NULL;
	}
	size_t find_scaling(float animation_time, const aiNodeAnim* node_animation);
	size_t find_rotation(float animation_time, const aiNodeAnim* node_animation);
	size_t find_position(float animation_time, const aiNodeAnim* node_animation);
	void calc_interpolated_scaling(aiVector3D& out, float animation_time, const aiNodeAnim* node_animation) {
		if (node_animation->mNumScalingKeys == 1) {
			out = node_animation->mScalingKeys[0].mValue;
			return;
		}
		size_t scaling_index = find_scaling(animation_time, node_animation);
		size_t next_scaling_index = scaling_index + 1;
		assert(next_scaling_index < node_animation->mNumScalingKeys);
		float delta_time = (float)(node_animation->mScalingKeys[next_scaling_index].mTime - node_animation->mScalingKeys[scaling_index].mTime);
		float factor = (animation_time - (float)node_animation->mScalingKeys[scaling_index].mTime) / delta_time;
		assert(factor >= 0.f && factor <= 1.f);
		const aiVector3D& start = node_animation->mScalingKeys[scaling_index].mValue;
		const aiVector3D& end = node_animation->mScalingKeys[next_scaling_index].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}
	void calc_interpolated_rotation(aiQuaternion& out, float animation_time, const aiNodeAnim* node_animation) {
		if (node_animation->mNumRotationKeys == 1) {
			out = node_animation->mRotationKeys[0].mValue;
			return;
		}
		size_t rotation_index = find_rotation(animation_time, node_animation);
		size_t next_rotation_index = rotation_index + 1;
		assert(next_rotation_index < node_animation->mNumRotationKeys);
		float delta_time = (float)(node_animation->mRotationKeys[next_rotation_index].mTime - node_animation->mRotationKeys[rotation_index].mTime);
		float factor = (animation_time - (float)node_animation->mRotationKeys[rotation_index].mTime) / delta_time;
		assert(factor >= 0.f && factor <= 1.f);
		const aiQuaternion& start = node_animation->mRotationKeys[rotation_index].mValue;
		const aiQuaternion& end = node_animation->mRotationKeys[next_rotation_index].mValue;
		aiQuaternion::Interpolate(out, start, end, factor);
		out = out.Normalize();
	}
	void calc_interpolated_position(aiVector3D& out, float animation_time, const aiNodeAnim* node_animation) {
		if (node_animation->mNumPositionKeys == 1) {
			out = node_animation->mPositionKeys[0].mValue;
			return;
		}
		size_t position_index = find_position(animation_time, node_animation);
		size_t next_position_index = position_index + 1;
		assert(next_position_index < node_animation->mNumPositionKeys);
		float delta_time = (float)(node_animation->mPositionKeys[next_position_index].mTime - node_animation->mPositionKeys[position_index].mTime);
		float factor = (animation_time - (float)node_animation->mPositionKeys[position_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		const aiVector3D& start = node_animation->mPositionKeys[position_index].mValue;
		const aiVector3D& end = node_animation->mPositionKeys[next_position_index].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}
	size_t find_scaling(float animation_time, const aiNodeAnim* node_animation) {
		assert(node_animation->mNumScalingKeys > 0);
		for (size_t i = 0; i < node_animation->mNumScalingKeys; i++) {
			if (animation_time < (float)node_animation->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}
		assert(false);
		return 0;
	}
	size_t find_rotation(float animation_time, const aiNodeAnim* node_animation) {
		assert(node_animation->mNumRotationKeys > 0);
		for (size_t i = 0; i < node_animation->mNumRotationKeys; i++) {
			if (animation_time < (float)node_animation->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}
		assert(false);
		return 0;
	}
	size_t find_position(float animation_time, const aiNodeAnim* node_animation) {
		assert(node_animation->mNumPositionKeys > 0);
		for (size_t i = 0; i < node_animation->mNumPositionKeys; i++) {
			if (animation_time < (float)node_animation->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}
		assert(false);
		return 0;
	}
}