#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class vertex_array_object {
		public:
			virtual void bind() = 0;
			virtual void unbind() = 0;
			BANANA_API virtual ~vertex_array_object();
			BANANA_API static vertex_array_object* create(graphics_api api = graphics_api::none);
		};
		class dummy_vao : public vertex_array_object {
		public:
			BANANA_API virtual void bind() override;
			BANANA_API virtual void unbind() override;
		};
	}
}