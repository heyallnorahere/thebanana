#include "pch.h"
#include "lua_interpreter.h"
#include "debug_tools.h"
#include "ui/ui.h"
#include "sound/sound.h"
#include "game.h"
namespace thebanana {
	int debug_print(lua_State* state) {
		std::string text = lua_tostring(state, 1);
		debug::log_print(text);
		return 0;
	}
	int load_menu(lua_State* state) {
		std::string path = lua_tostring(state, 1);
		g_game->get_menu_manager()->load_menu(new ui::menu(path));
		return 0;
	}
	int close_menus(lua_State* state) {
		g_game->get_menu_manager()->close_menus();
		return 0;
	}
	int close_game(lua_State* state) {
		g_game->destroy();
		return 0;
	}
	int play_sound(lua_State* state) {
		std::string name = lua_tostring(state, 1);
		bool repeat = lua_toboolean(state, 2);
		g_game->get_sound_manager()->get_sound(name)->play(repeat);
		return 0;
	}
	lua_interpreter::lua_interpreter() {
		this->init_state();
	}
	lua_interpreter::~lua_interpreter() {
		this->destroy_state();
	}
	void lua_interpreter::open_file(const std::string& path) {
		if (!check_lua(luaL_dofile(this->m_state, path.c_str()))) {
			debug::log_print("could not load " + path + ": " + lua_tostring(this->m_state, -1));
		}
	}
	struct lua_param_base {
	public:
		virtual void push() = 0;
	};
	template<typename _Ty> struct lua_param : public lua_param_base {
	public:
		lua_param(va_list& vl, lua_State* state);
		virtual void push() override;
	private:
		_Ty val;
		lua_State* state;
	};
	void lua_interpreter::call_function(const std::string& name, std::vector<param_type> types, ...) {
		lua_getglobal(this->m_state, name.c_str());
		if (!lua_isfunction(this->m_state, -1)) return;
		va_list vl;
		va_start(vl, types);
		for (size_t i = 0; i < types.size(); i++) {
			lua_param_base* param = NULL;
			param_type pt = types[i];
			switch (pt._type) {
			case type::_char:
				if (pt._unsigned) {
					param = new lua_param<unsigned char>(vl, this->m_state);
				}
				else {
					param = new lua_param<char>(vl, this->m_state);
				}
				break;
			case type::_short:
				if (pt._unsigned) {
					param = new lua_param<unsigned short>(vl, this->m_state);
				}
				else {
					param = new lua_param<short>(vl, this->m_state);
				}
				break;
			case type::_int:
				if (pt._unsigned) {
					param = new lua_param<unsigned int>(vl, this->m_state);
				}
				else {
					param = new lua_param<int>(vl, this->m_state);
				}
				break;
			case type::_long_long:
				if (pt._unsigned) {
					param = new lua_param<unsigned long long>(vl, this->m_state);
				}
				else {
					param = new lua_param<long long>(vl, this->m_state);
				}
				break;
			case type::ptr:
				param = new lua_param<size_t>(vl, this->m_state);
				break;
			default:
				debug::log_print("unrecognized parameter format, aborting lua call: " + name + "()");
				break;
			}
			assert(param);
			param->push();
			delete param;
		}
		va_end(vl);
		if (!check_lua(lua_pcall(this->m_state, types.size(), 1, 0))) {
			debug::log_print("could not call " + name + "(): " + lua_tostring(this->m_state, -1));
		}
	}
	void lua_interpreter::register_function(const std::string& name, func ptr) {
		for (const auto& fn : this->m_registered_functions) {
			if (name == fn) return;
		}
		lua_register(this->m_state, name.c_str(), ptr);
		this->m_registered_functions.push_back(name);
	}
	bool lua_interpreter::check_lua(int return_value) {
		return return_value == LUA_OK;
	}
	void lua_interpreter::init_state() {
		this->m_state = luaL_newstate();
		luaL_openlibs(this->m_state);
		this->add_engine_functions();
	}
	void lua_interpreter::destroy_state() {
		lua_close(this->m_state);
	}
	void lua_interpreter::reset_state() {
		this->destroy_state();
		this->init_state();
	}
	void lua_interpreter::add_engine_functions() {
		this->register_function("debug_print", debug_print);
		this->register_function("load_menu", load_menu);
		this->register_function("close_menus", close_menus);
		this->register_function("close_game", close_game);
		this->register_function("play_sound", play_sound);
	}
	template<typename _Ty> lua_param<_Ty>::lua_param(va_list& vl, lua_State* state) : state(state) {
		va_list vl_ = vl;
		this->val = va_arg(vl_, _Ty);
		vl = vl_;
	}
	template<typename _Ty> void lua_param<_Ty>::push() {
		lua_pushnumber(this->state, this->val);
	}
}