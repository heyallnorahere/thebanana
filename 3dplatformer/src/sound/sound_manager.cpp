#include "pch.h"
#include "sound/sound_manager.h"
#include "sound/sound_effect.h"
#include "sound/context.h"
#include "game.h"
#include "load_sound.h"
#include "debug_tools.h"
namespace thebanana {
	namespace sound {
		sound_manager::sound_manager(game* g_game) : m_game(g_game) {
			this->m_context = context::create();
			this->m_context->initialize();
		}
		sound_effect* sound_manager::load_sound(const std::string& name, const std::string& path) {
			size_t samples;
			unsigned char channels;
			unsigned int samples_per_second;
			short* buffer = ::load_sound(path, samples, channels, samples_per_second);
			if (!buffer) {
				debug::log_print("failed to load sound, aborting");
				return NULL;
			}
			this->m_sounds[name] = sound_effect::create(buffer, samples, channels, samples_per_second);
			free(buffer);
			return this->m_sounds[name];
		}
		sound_effect* sound_manager::get_sound(const std::string& name) {
			return this->m_sounds[name];
		}
		void sound_manager::delete_sound(const std::string& name) {
			delete this->m_sounds[name];
			this->m_sounds.erase(name);
		}
		sound_manager::~sound_manager() {
			for (auto p : this->m_sounds) {
				delete p.second;
			}
			this->m_context->destroy();
			delete this->m_context;
		}
		void sound_manager::init_decoders() {
			::init_decoders();
		}
	}
}