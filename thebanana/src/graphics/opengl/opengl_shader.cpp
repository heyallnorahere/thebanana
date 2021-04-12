#include "pch.h"
#include "opengl_shader.h"
#include "game.h"
static std::string read_file(const std::string& path) {
	std::ifstream file(path);
	std::stringstream content;
	std::string line;
	while (std::getline(file, line)) {
		content << line << '\n';
	}
	file.close();
	return content.str();
}
static unsigned int create_shader(const std::string& source, unsigned int type) {
	const char* src = source.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	int succeeded;
	glGetShaderiv(id, GL_COMPILE_STATUS, &succeeded);
	assert(succeeded);
	return id;
}
namespace thebanana {
	namespace graphics {
		namespace opengl {
			opengl_shader::opengl_shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path) : shader(vertex_path, fragment_path, geometry_path) {
				this->m_vertex_path = vertex_path;
				this->m_fragment_path = fragment_path;
				this->m_geometry_path = geometry_path;
				this->init();
			}
			opengl_shader::opengl_shader(void* id) : shader(id) {
				this->m_id = (size_t)id;
			}
			opengl_shader::~opengl_shader() {
				if (this->m_original) {
					g_game->debug_print("deleting shader: " + std::to_string(this->m_id));
					glDeleteProgram(this->m_id);
				}
			}
			void opengl_shader::bind() {
				glUseProgram(this->m_id);
			}
			void opengl_shader::unbind() {
				glUseProgram(NULL);
			}
			void* opengl_shader::get_id() {
				return (void*)(size_t)this->m_id;
			}
			void opengl_shader::reload() {
				if (this->m_original) {
					glDeleteProgram(this->m_id);
					this->init();
				}
			}
			void opengl_shader::init() {
				bool has_geometry_shader = !this->m_geometry_path.empty();
				unsigned int vs = create_shader(read_file(this->m_vertex_path), GL_VERTEX_SHADER);
				unsigned int fs = create_shader(read_file(this->m_fragment_path), GL_FRAGMENT_SHADER);
				unsigned int gs;
				if (has_geometry_shader) gs = create_shader(read_file(this->m_geometry_path), GL_GEOMETRY_SHADER);
				// create and link the program...
				this->m_id = glCreateProgram();
				glAttachShader(this->m_id, vs);
				glAttachShader(this->m_id, fs);
				if (has_geometry_shader) glAttachShader(this->m_id, gs);
				glLinkProgram(this->m_id);
				// check the status...
				int succeeded;
				glGetProgramiv(this->m_id, GL_LINK_STATUS, &succeeded);
				if (!succeeded) {
					char buf[256];
					glGetProgramInfoLog(this->m_id, 256 * sizeof(char), NULL, buf);
					std::cout << buf << std::endl;
					assert(false);
				}
				// and clean up after yourself!
				glDeleteShader(vs);
				glDeleteShader(fs);
				if (has_geometry_shader) glDeleteShader(gs);
			}
			void opengl_shader::uniform_int(const std::string& name, int val) {
				glUniform1i(glGetUniformLocation(this->m_id, name.c_str()), val);
			}
			void opengl_shader::uniform_float(const std::string& name, float val) {
				glUniform1f(glGetUniformLocation(this->m_id, name.c_str()), val);
			}
			void opengl_shader::uniform_vec3(const std::string& name, glm::vec3 val) {
				glUniform3f(glGetUniformLocation(this->m_id, name.c_str()), val.x, val.y, val.z);
			}
			void opengl_shader::uniform_vec4(const std::string& name, glm::vec4 val) {
				glUniform4f(glGetUniformLocation(this->m_id, name.c_str()), val.x, val.y, val.z, val.w);
			}
			void opengl_shader::uniform_mat4(const std::string& name, const glm::mat4& val, bool transpose) {
				glUniformMatrix4fv(glGetUniformLocation(this->m_id, name.c_str()), 1, transpose, glm::value_ptr(val));
			}
		}
	}
}