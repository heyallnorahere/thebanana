#include "pch.h"
#include "graphics/vertex_array_object.h"
#include "opengl/opengl_vao.h"
namespace thebanana {
	namespace graphics {
		vertex_array_object::~vertex_array_object() { }
		vertex_array_object* vertex_array_object::create(graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_vao;
				break;
			case graphics_api::opengl:
				return new opengl::opengl_vao;
				break;
			}
			return NULL;
		}
		void dummy_vao::bind() { }
		void dummy_vao::unbind() { }
	}
}