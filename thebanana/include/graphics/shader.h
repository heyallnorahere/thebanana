#pragma once
#include "graphics_types.h"
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		class shader {
		public:
			BANANA_API shader(const std::string& vertex_path, const std::string& pixel_path, const std::string& geometry_path = "");
			BANANA_API shader(void* id);
			BANANA_API virtual ~shader();
			virtual void bind() = 0;
			virtual void unbind() = 0;
			virtual void* get_id() = 0;
			virtual void reload() = 0;
			virtual void uniform_int(const std::string& name, int val) = 0;
			virtual void uniform_float(const std::string& name, float val) = 0;
			virtual void uniform_vec3(const std::string& name, glm::vec3 val) = 0;
			virtual void uniform_vec4(const std::string& name, glm::vec4 val) = 0;
			virtual void uniform_mat4(const std::string& name, const glm::mat4& val, bool transpose = false) = 0;
			BANANA_API static shader* create(const std::string& vertex_path, const std::string& pixel_path, const std::string& geometry_path = "", graphics_api api = graphics_api::none);
			BANANA_API static shader* create(void* id, graphics_api api = graphics_api::none);
		protected:
			bool m_original;
		};
	}
}