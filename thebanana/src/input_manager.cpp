#include "pch.h"
#include "input_manager.h"
#include "util.h"
#include "controller.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"
#include "internal_util.h"
#ifndef BANANA_WINDOWS
#define DIENUM_CONTINUE 0
#endif
namespace thebanana {
	input_manager::input_manager(game* g_game) : m_game(g_game) {
#ifdef BANANA_WINDOWS // temporary
		HRESULT hr = DirectInput8Create(get_current_module(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& this->m_context, NULL);
		if (FAILED(hr)) {
			this->m_context = NULL;
			return;
		}
		this->m_env = new dinput_callback_env;
		this->m_env->im = this;
		this->enum_devices();
#endif
	}
	void input_manager::enum_devices() {
#ifdef BANANA_WINDOWS
		this->m_env->time = CURRENT_TIME(double);
		this->enumerated_devices.clear();
		this->m_context->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
		this->m_context->EnumDevices(DI8DEVCLASS_KEYBOARD, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
		this->m_context->EnumDevices(DI8DEVCLASS_POINTER, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
#endif
	}
	std::vector<input_manager::dinput_device> input_manager::get_enumerated_devices() {
		return this->enumerated_devices;
	}
	input_manager::~input_manager() {
#ifdef BANANA_WINDOWS
		delete this->m_env;
		this->m_context->Release();
#endif
	}
	input_manager::device_type input_manager::add_device(size_t index) {
#ifdef BANANA_WINDOWS
		device* _device;
		device_type type = get_device_type(this->enumerated_devices[index]);
		switch (type) {
		case device_type::controller:
			_device = new controller;
			break;
		case device_type::keyboard:
			_device = new keyboard;
			break;
		case device_type::mouse:
			_device = new mouse;
			break;
		default:
			return type;
		}
		if (!_device->init(this->enumerated_devices[index], this->m_context, this)) return device_type::does_not_exist;
		this->devices.push_back(std::unique_ptr<device>(_device));
		return type;
#else
		return device_type::other;
#endif
	}
	input_manager::device_type input_manager::add_device(const std::string& name) {
#ifdef BANANA_WINDOWS
		for (size_t i = 0; i < this->enumerated_devices.size(); i++) {
			if (std::string(this->enumerated_devices[i].inst.tszProductName) == name) {
				return this->add_device(i);
			}
		}
#endif
		return device_type::does_not_exist;
	}
	void input_manager::update_devices() {
#ifdef BANANA_WINDOWS
		this->devices.remove_if([](const std::unique_ptr<device>& obj) { return !obj->connected(); });
		if (platform_specific::get_focus() != this->m_game->get_window().m) return;
		for (auto& d : this->devices) {
			d->update();
		}
#endif
	}
	size_t input_manager::get_num_devices() {
		return this->devices.size();
	}
	std::vector<input_manager::device::button> input_manager::get_device_buttons(size_t index) {
		auto it = this->devices.begin();
		std::advance(it, index);
		const auto& d = *it;
		return d->get_buttons();
	}
	input_manager::device_type input_manager::get_device_type(const dinput_device& dev) {
#ifdef BANANA_WINDOWS
		unsigned int dt = dev.inst.dwDevType & 0b11111;
		if (dt == DI8DEVTYPE_GAMEPAD)
			return device_type::controller;
		else if (dt == DI8DEVTYPE_KEYBOARD)
			return device_type::keyboard;
		else if (dt == DI8DEVTYPE_MOUSE)
			return device_type::mouse;
		else return device_type::other;
#else
		return device_type::other;
#endif
	}
	input_manager::device_type input_manager::get_device_type(size_t index) {
		auto it = this->devices.begin();
		std::advance(it, index);
		const auto& d = *it;
		return d->get_type();
	}
	input_manager::device* input_manager::get_device(size_t index) {
		auto it = this->devices.begin();
		std::advance(it, index);
		const auto& d = *it;
		return d.get();
	}
	game* input_manager::get_parent() {
		return this->m_game;
	}
	input_manager::device::button input_manager::get_key(key k) {
		size_t pos = this->find_device(device_type::keyboard);
		assert(pos != std::string::npos);
		auto btns = this->get_device_buttons(pos);
		if ((int)k >= btns.size()) {
			return { false, false, false };
		}
		return btns[(int)k];
	}
	input_manager::device::button input_manager::get_mouse_button(mouse_button mb) {
		size_t pos = this->find_device(device_type::mouse);
		if (pos == std::string::npos) {
			return { false, false, false };
		}
		auto btns = this->get_device_buttons(pos);
		if ((int)mb >= btns.size()) {
			return { false, false, false };
		}
		return btns[(int)mb];
	}
	size_t input_manager::find_device(device_type type) {
		size_t pos = std::string::npos;
		for (size_t i = 0; i < this->get_num_devices(); i++) {
			if (this->get_device_type(i) == type) {
				pos = i;
				break;
			}
		}
		return pos;
	}
	int __stdcall input_manager::enum_callback(const DIDEVICEINSTANCEA* inst, dinput_callback_env* passed_env) {
		dinput_callback_env env = *passed_env;
		env.im->enumerated_devices.push_back({ *inst, env });
		return DIENUM_CONTINUE;
	}
	bool input_manager::device::init(const dinput_device& device, IDirectInput8A* context, input_manager* parent) {
#ifdef BANANA_WINDOWS
		this->parent = parent;
		HRESULT hr = context->CreateDevice(device.inst.guidInstance, &this->device, NULL);
		if (FAILED(hr)) return false;
		this->device->SetDataFormat(this->get_format());
		this->device->Acquire();
		this->device_specific_init();
#endif
		return true;
	}
	input_manager::device::~device() {
#ifdef BANANA_WINDOWS
		this->device->Unacquire();
		this->device->Release();
#endif
	}
}