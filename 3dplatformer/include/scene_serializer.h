#pragma once
namespace thebanana {
	class scene;
	class scene_serializer {
	public:
		scene_serializer(scene* scene);
		void serialize(const std::string& path);
		void deserialize(const std::string& path);
	private:
		scene* m_scene;
	};
}