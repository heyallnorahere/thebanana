#pragma once
#include "misc.h"
namespace thebanana {
	namespace sound {
		class context {
		public:
			virtual void initialize() = 0;
			virtual void destroy() = 0;
			static context* create(sound_api api = sound_api::none);
		};
		class dummy_context : public context {
		public:
			virtual void initialize() override;
			virtual void destroy() override;
		};
		class openal_context : public context {
		public:
			virtual void initialize() override;
			virtual void destroy() override;
		private:
#ifndef BANANA_BUILD
			using ALCdevice = void;
			using ALCcontext = void;
#endif
			ALCdevice* m_device;
			ALCcontext* m_context;
		};
	}
}