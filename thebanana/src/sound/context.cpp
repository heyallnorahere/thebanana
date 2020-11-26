#include "pch.h"
#include "sound/context.h"
namespace thebanana {
	namespace sound {
		context* context::create(sound_api api_) {
			sound_api api = api_;
			if (api == sound_api::none) {
				api = get_default_sound_api();
			}
			switch (api) {
			case sound_api::none:
				return new dummy_context;
				break;
			case sound_api::openal:
				return new openal_context;
				break;
			}
			return NULL;
		}
		void dummy_context::initialize() { }
		void dummy_context::destroy() { }
		void openal_context::initialize() {
			this->m_device = alcOpenDevice(NULL);
			this->m_context = alcCreateContext(this->m_device, NULL);
			alcMakeContextCurrent(this->m_context);
		}
		void openal_context::destroy() {
			alcDestroyContext(this->m_context);
			this->m_context = nullptr;
			alcCloseDevice(this->m_device);
			this->m_device = nullptr;
		}
	}
}