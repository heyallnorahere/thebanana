#pragma once
#include "banana_api.h"
namespace thebanana {
	namespace sound {
		enum class sound_api {
			none,
			openal,
		};
		BANANA_API void set_default_sound_api(sound_api api);
		BANANA_API sound_api get_default_sound_api();
	}
}