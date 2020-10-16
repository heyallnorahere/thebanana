#include "pch.h"
#include "load_sound.h"
#define MINIMP3_IMPLEMENTATION
#define MINIMP3_ONLY_MP3
#include <minimp3_ex.h>
#include <vorbis/vorbisfile.h>
enum class format {
	mp3,
	ogg,
	other,
};
static format get_sound_format(const std::string& extension) {
	if (extension == "mp3") {
		return format::mp3;
	} else if (extension == "ogg") {
		return format::ogg;
	} else {
		return format::other;
	}
}
mp3dec_t mp3d;
void init_decoders() {
	mp3dec_init(&mp3d);
}
short* load_mp3(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second);
short* load_ogg(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second);
short* load_sound(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second) {
	size_t period_pos = path.find_last_of('.');
	if (period_pos == std::string::npos) return NULL;
	format fmt = get_sound_format(path.substr(period_pos + 1));
	switch (fmt) {
	case format::mp3:
		return load_mp3(path, samples, channels, samples_per_second);
		break;
	case format::ogg:
		return load_ogg(path, samples, channels, samples_per_second);
		break;
	}
	return NULL;
}
short* load_mp3(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second) {
	mp3dec_file_info_t info;
	if (mp3dec_load(&mp3d, path.c_str(), &info, NULL, NULL)) {
		return NULL;
	}
	channels = (unsigned char)info.channels;
	samples_per_second = (unsigned int)info.hz;
	samples = info.samples;
	size_t buffer_length = samples * sizeof(short);
	short* buffer = (short*)malloc(buffer_length);
	assert(buffer);
	memcpy(buffer, info.buffer, buffer_length);
	return buffer;
}
short* load_ogg(const std::string& path, size_t& samples, unsigned char& channels, unsigned int& samples_per_second) {
	OggVorbis_File ovf;
	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp) return NULL;
	ov_open_callbacks(fp, &ovf, NULL, 0, OV_CALLBACKS_NOCLOSE);
	vorbis_info* vi = ov_info(&ovf, -1);
	channels = (unsigned char)vi->channels;
	samples_per_second = (unsigned int)vi->rate;
	samples = ov_pcm_total(&ovf, -1) * channels;
	size_t buffer_length = samples * sizeof(short);
	short* buffer = (short*)malloc(buffer_length);
	assert(buffer);
	memset(buffer, 0, buffer_length);
	for (size_t size = 0, offset = 0, sel = 0; (size = ov_read(&ovf, (char*)buffer + offset, 4096, 0, sizeof(short), true, (int*)&sel)); offset += size) {
		if (size < 0)
			return NULL;
	}
	ov_clear(&ovf);
	fclose(fp);
	return buffer;
}
