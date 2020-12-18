#pragma once
#include "graphics/vertex_buffer.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_vbo : public vertex_buffer {
			public:
				opengl_vbo(const data& d);
				virtual void bind() override;
				virtual void unbind() override;
				virtual ~opengl_vbo() override;
			private:
				void setup();
				unsigned int m_id;
			};
		}
	}
}