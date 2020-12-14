#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class framebuffer {
		public:
			struct specification;
			struct attachment_settings {
				unsigned int type = 0;
				unsigned int internal_format = 0;
				unsigned int format = 0;
				unsigned int attachment_type = 0;
				unsigned int min_filter = 0;
				unsigned int mag_filter = 0;
				unsigned int wrap_s = 0;
				unsigned int wrap_t = 0;
				unsigned int wrap_r = 0;
				void(*texture_proc)(specification* spec) = NULL;
				void(*framebuffer_attachment_proc)(unsigned int id, specification* spec) = NULL;
			};
			struct specification {
				static constexpr int color = 0b1, depth = 0b10;
				int width, height, buffers;
				attachment_settings color_settings, depth_settings;
			};
			struct attachment_map {
				int color_index = -1;
				int depth_index = -1;
			};
			struct attachment {
				enum type {
					color,
					depth,
				};
				type type;
				void* value;
			};
			BANANA_API framebuffer(void* data);
			virtual void bind() = 0;
			virtual void unbind() = 0;
			virtual void set_size(int width, int height) = 0;
			virtual void reload() = 0;
			virtual void* get_id() = 0;
			virtual std::vector<attachment> get_attachments() = 0;
			BANANA_API virtual ~framebuffer();
			BANANA_API attachment_map get_attachment_map() const;
			BANANA_API static framebuffer* create(specification* spec, graphics_api api = graphics_api::none);
		protected:
			attachment_map m_attachment_map;
		};
		class dummy_framebuffer : public framebuffer {
		public:
			BANANA_API dummy_framebuffer(void* data);
			BANANA_API virtual void bind() override;
			BANANA_API virtual void unbind() override;
			BANANA_API virtual void set_size(int width, int height) override;
			BANANA_API virtual void reload() override;
			BANANA_API virtual void* get_id() override;
			BANANA_API virtual std::vector<attachment> get_attachments() override;
		};
	}
}