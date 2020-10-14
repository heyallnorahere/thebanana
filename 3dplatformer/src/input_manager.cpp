#include "pch.h"
#include "input_manager.h"
#include "util.h"
#include "controller.h"
#include "keyboard.h"
#include "mouse.h"
namespace thebanana {
	input_manager::input_manager(game* g_game) : m_game(g_game) {
		HRESULT hr = DirectInput8Create(HINST_THISCOMPONENT, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& this->m_context, NULL);
		if (FAILED(hr)) {
			this->m_context = NULL;
			return;
		}
		this->m_env = new dinput_callback_env;
		this->m_env->im = this;
		this->enum_devices();
	}
	void input_manager::enum_devices() {
		this->m_env->time = CURRENT_TIME(double);
		this->enumerated_devices.clear();
		this->m_context->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
		this->m_context->EnumDevices(DI8DEVCLASS_KEYBOARD, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
		this->m_context->EnumDevices(DI8DEVCLASS_POINTER, (LPDIENUMDEVICESCALLBACK)enum_callback, this->m_env, DIEDFL_ATTACHEDONLY);
	}
	std::vector<input_manager::dinput_device> input_manager::get_enumerated_devices() {
		return this->enumerated_devices;
	}
	input_manager::~input_manager() {
		delete this->m_env;
		this->m_context->Release();
	}
	input_manager::device_type input_manager::add_device(size_t index) {
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
		if (!_device->init(this->enumerated_devices[index], this->m_context)) return device_type::does_not_exist;
		this->devices.push_back(std::unique_ptr<device>(_device));
		return type;
	}
	input_manager::device_type input_manager::add_device(const win32_string& name) {
		for (size_t i = 0; i < this->enumerated_devices.size(); i++) {
			if (win32_string(this->enumerated_devices[i].inst.tszProductName) == name) {
				return this->add_device(i);
			}
		}
		return device_type::does_not_exist;
	}
	void input_manager::update_devices() {
		this->devices.remove_if([](const std::unique_ptr<device>& obj) { return !obj->connected(); });
		for (auto& d : this->devices) {
			d->update();
		}
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
		unsigned int dt = dev.inst.dwDevType & 0b11111;
		if (dt == DI8DEVTYPE_GAMEPAD)
			return device_type::controller;
		else if (dt == DI8DEVTYPE_KEYBOARD)
			return device_type::keyboard;
		else if (dt == DI8DEVTYPE_MOUSE)
			return device_type::mouse;
		else return device_type::other;
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
	int __stdcall input_manager::enum_callback(const DIDEVICEINSTANCE* inst, dinput_callback_env* passed_env) {
		dinput_callback_env env = *passed_env;
		env.im->enumerated_devices.push_back({ *inst, env });
		return DIENUM_CONTINUE;
	}
	bool input_manager::device::init(const dinput_device& device, IDirectInput8* context) {
		HRESULT hr = context->CreateDevice(device.inst.guidInstance, &this->device, NULL);
		if (FAILED(hr)) return false;
		this->device->SetDataFormat(this->get_format());
		this->device->Acquire();
		this->device_specific_init();
		return true;
	}
	input_manager::device::~device() {
		this->device->Unacquire();
		this->device->Release();
	}
}