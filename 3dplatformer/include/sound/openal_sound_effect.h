#pragma once
#include "sound_effect.h"
namespace thebanana {
	namespace sound {
		class openal_sound_effect : public sound_effect {
		public:
			openal_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			virtual void play(bool repeat) override;
			virtual void pause() override;
			virtual void stop() override;
			virtual bool is_playing() override;
			virtual bool is_paused() override;
			virtual ~openal_sound_effect() override;
		private:
			void create_al_objects();
			void destroy_al_objects();
			unsigned int source, buffer;
		};
	}
}