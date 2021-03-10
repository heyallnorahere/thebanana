#pragma once
#include "graphics_types.h"
#include "game.h"
namespace thebanana {
	namespace platform_specific {
		struct gl_context_t;
	}
	namespace graphics {
		class context {
		public:
			struct opengl_data {
				int major_version, minor_version;
			};
			BANANA_API context(const window& w);
			virtual void swap_buffers() = 0;
			virtual void bind() = 0;
			virtual void unbind() = 0;
			BANANA_API virtual ~context();
			BANANA_API static context* create(const window& w, void* data, graphics_api api = graphics_api::none);
		protected:
			window m_window;
			platform_specific::gl_context_t* m_context;
		};
	}
}