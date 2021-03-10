#include "pch.h"
#include "opengl_quad.h"
#include "game.h"
#include "shader_registry.h"
#include "graphics/util.h"
struct vertex {
	glm::vec3 pos;
	glm::vec2 uv;
};
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_quad::opengl_quad(float width, float height, texture* tex, bool invert_uv) : quad(width, height, tex, invert_uv) {
				this->init_quad(invert_uv);
			}
			opengl_quad::~opengl_quad() {
				this->delete_quad();
			}
			void opengl_quad::init_shader(const std::string& name) {
				quad_shader = g_game->get_shader_registry()->get_shader(name);
			}
			void opengl_quad::api_render() {
				unsigned int shader = util::get_current_shader();
				quad_shader->bind();
				glDisable(GL_DEPTH_TEST);
				glBindVertexArray(this->m_vao);
				glDrawElements(GL_TRIANGLES, this->m_index_count, GL_UNSIGNED_INT, NULL);
				glBindVertexArray(NULL);
				glEnable(GL_DEPTH_TEST);
				glUseProgram(shader);
			}
			void opengl_quad::init_quad(bool invert_uv) {
				std::vector<vertex> vertices = {
					 { glm::vec3(-(this->m_width / 2.f), -(this->m_height / 2.f), 0.f), glm::vec2(0.f, invert_uv ? 1.f : 0.f) },
					 { glm::vec3( (this->m_width / 2.f), -(this->m_height / 2.f), 0.f), glm::vec2(1.f, invert_uv ? 1.f : 0.f) },
					 { glm::vec3( (this->m_width / 2.f),  (this->m_height / 2.f), 0.f), glm::vec2(1.f, invert_uv ? 0.f : 1.f) },
					 { glm::vec3(-(this->m_width / 2.f),  (this->m_height / 2.f), 0.f), glm::vec2(0.f, invert_uv ? 0.f : 1.f) },
				};
				std::vector<unsigned int> indices = {
					0, 1, 3,
					1, 2, 3,
				};
				this->m_index_count = indices.size();
				glGenVertexArrays(1, &this->m_vao);
				glBindVertexArray(this->m_vao);
				glGenBuffers(1, &this->m_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
				glGenBuffers(1, &this->m_ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), (void*)offsetof(vertex, pos));
				glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(vertex), (void*)offsetof(vertex, uv));
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glBindVertexArray(NULL);
			}
			void opengl_quad::delete_quad() {
				glDeleteBuffers(1, &this->m_ebo);
				glDeleteBuffers(1, &this->m_vbo);
				glDeleteVertexArrays(1, &this->m_vao);
			}
			shader* opengl_quad::quad_shader = NULL;
		}
	}
}