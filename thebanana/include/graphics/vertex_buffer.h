#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class vertex_buffer {
		public:
			struct data {
				struct vertex_attrib {
					enum _type {
						type_float,
						type_uint,
						type_int,
						type_ubyte,
						type_byte,
					};
					size_t size;
					_type type;
					bool normalize;
					unsigned int elements;
				};
				std::vector<vertex_attrib> attributes;
				const void* data;
				size_t size;
			};
			BANANA_API vertex_buffer(const data& d);
			virtual void bind() = 0;
			virtual void unbind() = 0;
			BANANA_API virtual ~vertex_buffer();
			BANANA_API static vertex_buffer* create(const data& d, graphics_api api = graphics_api::none);
		protected:
			data m_data;
		};
	}
}