#pragma once
#include "misc.h"
#include "banana_api.h"
namespace thebanana {
	namespace sound {
		class context {
		public:
			virtual void initialize() = 0;
			virtual void destroy() = 0;
			BANANA_API static context* create(sound_api api = sound_api::none);
		};
		class dummy_context : public context {
		public:
			BANANA_API virtual void initialize() override;
			BANANA_API virtual void destroy() override;
		};
		class openal_context : public context {
		public:
			BANANA_API virtual void initialize() override;
			BANANA_API virtual void destroy() override;
		private:
#ifndef BANANA_BUILD
			using ALCdevice = void;
			using ALCcontext = void;
#endif
			ALCdevice* m_device = nullptr;
			ALCcontext* m_context = nullptr;
		};
	}
}