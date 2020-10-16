#include "pch.h"
#include "sound/misc.h"
namespace thebanana {
	namespace sound {
		sound_api default_api = sound_api::none;
		void set_default_sound_api(sound_api api) {
			default_api = api;
		}
		sound_api get_default_sound_api() {
			return default_api;
		}
	}
}