#pragma once
#include "graphics_types.h"
#include "gameobject.h"
namespace thebanana {
	namespace graphics {
		class texture;
		class quad : public gameobject {
		public:
			quad(float width, float height, texture* tex, bool invert_uv);
			virtual void update() override;
			virtual void render() override;
			static quad* create(float width, float height, texture* tex, bool invert_uv = false, graphics_api api = graphics_api::none);
		protected:
			virtual void api_render() = 0;
			float m_width, m_height;
		private:
			texture* m_texture;
		};
		class dummy_quad : public quad {
		public:
			dummy_quad(float width, float height, texture* tex, bool invert_uv);
		protected:
			virtual void api_render() override;
		};
	}
}