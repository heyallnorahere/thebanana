#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class index_buffer {
		public:
			struct data {
				enum _type {
					type_uint,
					type_int,
					type_ubyte,
					type_byte,
				};
				const void* data;
				size_t size, index_count;
				_type type;
			};
			BANANA_API index_buffer(const data& d);
			virtual void bind() = 0;
			virtual void unbind() = 0;
			virtual void draw() = 0;
			BANANA_API virtual ~index_buffer();
			BANANA_API static index_buffer* create(const data& d, graphics_api api = graphics_api::none);
		protected:
			data m_data;
		};
	}
}