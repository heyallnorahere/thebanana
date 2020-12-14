#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class texture {
		public:
			struct data {
				int width, height, channels;
				void* pixels;
			};
			BANANA_API texture(const data& d);
			virtual void bind(unsigned int slot = 0) = 0;
			virtual void unbind() = 0;
			virtual void set_data(void* pixels) = 0;
			// buffer must have a size of at least width * height * channels
			virtual void get_data(void* buffer) = 0;
			BANANA_API virtual ~texture();
			BANANA_API int get_width() const;
			BANANA_API int get_height() const;
			BANANA_API int get_channels() const;
			BANANA_API unsigned int get_id() const;
			BANANA_API static texture* create(const data& d, void* api_specific = NULL, graphics_api api = graphics_api::none);
		protected:
			int m_width, m_height, m_channels;
			unsigned int id;
		};
		class dummy_texture : public texture {
		public:
			BANANA_API dummy_texture(const data& d);
			BANANA_API virtual void bind(unsigned int slot = 0) override;
			BANANA_API virtual void unbind() override;
			BANANA_API virtual void set_data(void* pixels) override;
			BANANA_API virtual void get_data(void* buffer) override;
		};
	}
}