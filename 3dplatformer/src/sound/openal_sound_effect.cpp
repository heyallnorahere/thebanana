#include "pch.h"
#include "sound/openal_sound_effect.h"
namespace thebanana {
	namespace sound {
		openal_sound_effect::openal_sound_effect(short* data, size_t samples, unsigned char channels, unsigned int samples_per_second) : sound_effect(data, samples, channels, samples_per_second) {
			this->create_al_objects();
		}
		void openal_sound_effect::play(bool repeat) {
			if (this->is_playing() && !this->is_paused()) {
				this->stop();
			}
			alSourcei(this->source, AL_LOOPING, repeat);
			alSourcePlay(this->source);
		}
		void openal_sound_effect::pause() {
			if (!this->is_playing()) return;
			if (!this->is_paused()) {
				alSourcePause(this->source);
			}
			else {
				alSourcePlay(this->source);
			}
		}
		void openal_sound_effect::stop() {
			if (!this->is_playing()) return;
			alSourceStop(this->source);
		}
		bool openal_sound_effect::is_playing() {
			int playing;
			alGetSourcei(this->source, AL_PLAYING, &playing);
			return playing;
		}
		bool openal_sound_effect::is_paused() {
			int paused;
			alGetSourcei(this->source, AL_PAUSED, &paused);
			return paused;
		}
		openal_sound_effect::~openal_sound_effect() {
			this->destroy_al_objects();
		}
		void openal_sound_effect::create_al_objects() {
			alGenSources(1, &this->source);
			int error = alGetError();
			if (error) __debugbreak();
			alGenBuffers(1, &this->buffer);
			error = alGetError();
			if (error) __debugbreak();
			alBufferData(this->buffer, this->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, this->data.data(), this->data.size() * sizeof(short), this->samples_per_second);
			alSourcei(this->source, AL_BUFFER, this->buffer);
		}
		void openal_sound_effect::destroy_al_objects() {
			alDeleteBuffers(1, &this->buffer);
			alDeleteSources(1, &this->source);
		}
	}
}