#pragma once
#include "misc.h"
namespace thebanana {
	namespace sound {
		class sound_effect {
		public:
			sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			virtual void play(bool repeat) = 0;
			virtual void pause() = 0;
			virtual void stop() = 0;
			virtual bool is_playing() = 0;
			virtual bool is_paused() = 0;
			virtual ~sound_effect();
			double get_length_in_seconds() const;
			size_t get_length_in_samples() const;
			unsigned char get_channels() const;
			unsigned int get_samples_per_second() const;
			static sound_effect* create(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second, sound_api api = sound_api::none);
		protected:
			std::vector<short> data;
			size_t samples;
			unsigned char channels;
			unsigned int samples_per_second;
		};
		class dummy_sound_effect : public sound_effect {
		public:
			dummy_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second);
			virtual void play(bool repeat) override;
			virtual void pause() override;
			virtual void stop() override;
			virtual bool is_playing() override;
			virtual bool is_paused() override;
		};
	}
}