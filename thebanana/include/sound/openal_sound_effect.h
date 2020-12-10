#pragma once
#include "sound_effect.h"
#include "banana_api.h"
namespace thebanana {
	namespace sound {
		class openal_sound_effect : public sound_effect {
		public:
			BANANA_API openal_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			BANANA_API virtual void play(bool repeat) override;
			BANANA_API virtual void pause() override;
			BANANA_API virtual void stop() override;
			BANANA_API virtual bool is_playing() override;
			BANANA_API virtual bool is_paused() override;
			BANANA_API virtual ~openal_sound_effect() override;
		private:
			void create_al_objects();
			void destroy_al_objects();
			unsigned int source, buffer;
		};
	}
}