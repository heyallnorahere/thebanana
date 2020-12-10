#pragma once
#include "graphics/framebuffer.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_framebuffer : public framebuffer {
			public:
				opengl_framebuffer(specification* s);
				virtual void bind() override;
				virtual void unbind() override;
				virtual void set_size(int width, int height) override;
				virtual void reload() override;
				virtual void* get_id() override;
				virtual std::vector<attachment> get_attachments() override;
				virtual ~opengl_framebuffer() override;
			private:
				specification m_specification;
				std::vector<attachment> m_attachments;
				unsigned int m_id;
				void setup();
				void clean_up();
			};
		}
	}
}