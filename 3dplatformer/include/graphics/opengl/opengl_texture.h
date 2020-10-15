#pragma once
#include "graphics/texture.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_texture : public texture {
			public:
				struct settings {
					int wrap_s = 0, wrap_t = 0, min_filter = 0, mag_filter = 0;
				};
				opengl_texture(const data& d, void* api_specific);
				virtual void bind(unsigned int slot = 0) override;
				virtual void unbind() override;
				virtual void set_data(void* pixels) override;
				virtual ~opengl_texture() override;
			private:
				settings m_settings;
				void create_texture(void* pixels);
				void destroy_texture();
				unsigned int last_bound_slot;
			};
		}
	}
}