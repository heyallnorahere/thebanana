#pragma once
namespace thebanana {
	class game;
	namespace sound {
		class sound_effect;
		class context;
		class sound_manager {
		public:
			sound_manager(game* g_game);
			sound_effect* load_sound(const std::string& name, const std::string& path);
			sound_effect* get_sound(const std::string& name);
			void delete_sound(const std::string& name);
			~sound_manager();
			static void init_decoders();
		private:
			game* m_game;
			context* m_context;
			std::map<std::string, sound_effect*> m_sounds;
		};
	}
}