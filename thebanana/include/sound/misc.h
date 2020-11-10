#pragma once
namespace thebanana {
	namespace sound {
		enum class sound_api {
			none,
			openal,
		};
		void set_default_sound_api(sound_api api);
		sound_api get_default_sound_api();
	}
}