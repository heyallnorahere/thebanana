#pragma once
#include "graphics_types.h"
namespace thebanana {
	namespace graphics {
		class texture {
		public:
			struct data {
				int width, height, channels;
				void* pixels;
			};
			texture(const data& d);
			virtual void bind(unsigned int slot = 0) = 0;
			virtual void unbind() = 0;
			virtual void set_data(void* pixels) = 0;
			virtual ~texture();
			int get_width() const;
			int get_height() const;
			int get_channels() const;
			unsigned int get_id() const;
			static texture* create(const data& d, void* api_specific = NULL, graphics_api api = graphics_api::none);
		protected:
			int m_width, m_height, m_channels;
			unsigned int id;
		};
		class dummy_texture : public texture {
		public:
			dummy_texture(const data& d);
			virtual void bind(unsigned int slot = 0) override;
			virtual void unbind() override;
			virtual void set_data(void* pixels) override;
		};
	}
}