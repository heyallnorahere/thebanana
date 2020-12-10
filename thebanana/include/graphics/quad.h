#pragma once
#include "graphics_types.h"
#include "gameobject.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class texture;
		class quad : public gameobject {
		public:
			BANANA_API quad(float width, float height, texture* tex, bool invert_uv);
			BANANA_API virtual void update() override;
			BANANA_API virtual void render() override;
			BANANA_API static quad* create(float width, float height, texture* tex, bool invert_uv = false, graphics_api api = graphics_api::none);
			BANANA_API static void init_opengl_shader(const std::string& name);
		protected:
			virtual void api_render() = 0;
			float m_width, m_height;
		private:
			texture* m_texture;
		};
		class dummy_quad : public quad {
		public:
			BANANA_API dummy_quad(float width, float height, texture* tex, bool invert_uv);
		protected:
			BANANA_API virtual void api_render() override;
		};
	}
}