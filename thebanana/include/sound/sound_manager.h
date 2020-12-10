#pragma once
#include "banana_api.h"
namespace thebanana {
	class game;
	namespace sound {
		class sound_effect;
		class context;
		class sound_manager {
		public:
			BANANA_API sound_manager(game* g_game);
			BANANA_API sound_effect* load_sound(const std::string& name, const std::string& path);
			BANANA_API sound_effect* get_sound(const std::string& name);
			BANANA_API void delete_sound(const std::string& name);
			BANANA_API ~sound_manager();
			BANANA_API static void init_decoders();
		private:
			game* m_game;
			context* m_context;
			std::map<std::string, sound_effect*> m_sounds;
		};
	}
}