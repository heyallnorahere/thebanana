#pragma once
#include "graphics/vertex_array_object.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_vao : public vertex_array_object {
			public:
				opengl_vao();
				virtual void bind() override;
				virtual void unbind() override;
				virtual ~opengl_vao() override;
			private:
				unsigned int m_id;
			};
		}
	}
}