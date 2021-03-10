#pragma once
#include "banana_api.h"
namespace thebanana {
	class game;
	namespace graphics {
		class shader;
	}
	class shader_registry {
	public:
		BANANA_API shader_registry(game* g_game);
		BANANA_API void register_shader(const std::string& name, graphics::shader* shader);
		BANANA_API void register_shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
		BANANA_API graphics::shader* get_shader(const std::string& name);
		BANANA_API ~shader_registry();
	private:
		std::map<std::string, graphics::shader*> m_shaders;
		game* m_game;
	};
}