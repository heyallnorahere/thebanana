#pragma once
#include "graphics/index_buffer.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_ebo : public index_buffer {
			public:
				opengl_ebo(const data& d);
				virtual void bind() override;
				virtual void unbind() override;
				virtual void draw() override;
				virtual ~opengl_ebo() override;
			private:
				void setup();
				unsigned int m_id;
			};
		}
	}
}