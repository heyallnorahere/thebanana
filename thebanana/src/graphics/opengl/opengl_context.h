#pragma once
#include "graphics/context.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_context : public context {
			public:
				opengl_context(const window& w, opengl_data* data);
				virtual void swap_buffers() override;
				virtual void bind() override;
				virtual void unbind() override;
				virtual ~opengl_context() override;
			};
		}
	}
}