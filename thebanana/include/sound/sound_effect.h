#pragma once
#include "misc.h"
#include "banana_api.h"
namespace thebanana {
	namespace sound {
		class sound_effect {
		public:
			BANANA_API sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			BANANA_API virtual void play(bool repeat) = 0;
			BANANA_API virtual void pause() = 0;
			BANANA_API virtual void stop() = 0;
			BANANA_API virtual bool is_playing() = 0;
			BANANA_API virtual bool is_paused() = 0;
			BANANA_API virtual ~sound_effect();
			BANANA_API double get_length_in_seconds() const;
			BANANA_API size_t get_length_in_samples() const;
			BANANA_API unsigned char get_channels() const;
			BANANA_API unsigned int get_samples_per_second() const;
			BANANA_API static sound_effect* create(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second, sound_api api = sound_api::none);
		protected:
			std::vector<short> data;
			size_t samples;
			unsigned char channels;
			unsigned int samples_per_second;
		};
		class dummy_sound_effect : public sound_effect {
		public:
			BANANA_API dummy_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			BANANA_API virtual void play(bool repeat) override;
			BANANA_API virtual void pause() override;
			BANANA_API virtual void stop() override;
			BANANA_API virtual bool is_playing() override;
			BANANA_API virtual bool is_paused() override;
		};
	}
}