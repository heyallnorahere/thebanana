#include "pch.h"
#include "sound/sound_effect.h"
#include "sound/openal_sound_effect.h"
namespace thebanana {
	namespace sound {
		sound_effect::sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second) {
			for (size_t i = 0; i < samples; i++) {
				this->data.push_back(data[i]);
			}
			this->samples = samples;
			this->channels = channels;
			this->samples_per_second = samples_per_second;
		}
		sound_effect::~sound_effect() { }
		double sound_effect::get_length_in_seconds() const {
			return static_cast<double>(this->samples) / static_cast<double>(this->samples_per_second * this->channels);
		}
		size_t sound_effect::get_length_in_samples() const {
			return this->samples / (size_t)this->channels;
		}
		unsigned char sound_effect::get_channels() const {
			return this->channels;
		}
		unsigned int sound_effect::get_samples_per_second() const {
			return this->samples_per_second;
		}
		sound_effect* sound_effect::create(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second, sound_api api_) {
			sound_api api = api_;
			if (api == sound_api::none) {
				api = get_default_sound_api();
			}
			switch (api) {
			case sound_api::none:
				return new dummy_sound_effect(data, samples, channels, samples_per_second);
				break;
			case sound_api::openal:
				return new openal_sound_effect(data, samples, channels, samples_per_second);
				break;
			}
			return NULL;
		}
		dummy_sound_effect::dummy_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second) : sound_effect(data, samples, channels, samples_per_second) { }
		void dummy_sound_effect::play(bool repeat) { }
		void dummy_sound_effect::pause() { }
		void dummy_sound_effect::stop() { }
		bool dummy_sound_effect::is_playing() { return false; }
		bool dummy_sound_effect::is_paused() { return false; }
	}
}