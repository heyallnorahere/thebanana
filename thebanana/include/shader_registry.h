#pragma once
#include "banana_api.h"
namespace opengl_shader_library {
	class shader;
}
namespace thebanana {
	class game;
	class shader_registry {
	public:
		BANANA_API shader_registry(game* g_game);
		BANANA_API void register_shader(const std::string& name, opengl_shader_library::shader* shader);
#ifdef BANANA_BUILD
		void register_shader(const std::string& name, int vertex, int fragment, int geometry = NULL);
#endif
		BANANA_API void register_shader(const std::string& name, const std::string& dir, bool geometry = false);
		BANANA_API opengl_shader_library::shader* get_shader(const std::string& name);
		BANANA_API ~shader_registry();
	private:
		std::map<std::string, opengl_shader_library::shader*> m_shaders;
		game* m_game;
	};
}