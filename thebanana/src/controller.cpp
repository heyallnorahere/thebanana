#include "pch.h"
#include "controller.h"
#include "input_utils.h"
#include "debug_tools.h"
namespace thebanana {
	controller::controller() : is_connected(true) { }
	controller::~controller() { }
	std::vector<controller::button> controller::get_buttons() {
		return this->current;
	}
	void controller::update() {
		this->is_connected = SUCCEEDED(this->device->Poll());
		this->device->GetDeviceState(sizeof(DIJOYSTATE), &this->state);
		for (size_t i = 0; i < this->buttons; i++) {
			this->last[i] = this->current[i].held;
		}
		float deadzone = 0.05f;
		calc_pressed(this->current, this->state, this->buttons);
		calc_down(this->current, this->last, this->buttons);
		calc_up(this->current, this->last, this->buttons);
		float x = dinput_controller_axis(this->state.val.lX) - 0.5f;
		if (abs(x) < deadzone) x = 0.f;
		float y = -(dinput_controller_axis(this->state.val.lY) - 0.5f);
		if (abs(y) < deadzone) y = 0.f;
		this->js.left = glm::vec2(x, y);
		x = (dinput_controller_axis(this->state.val.lRx) - 0.5f);
		if (abs(x) < deadzone) x = 0.f;
		y = -(dinput_controller_axis(this->state.val.lRy) - 0.5f);
		if (abs(y) < deadzone) y = 0.f;
		this->js.right = glm::vec2(x, y);
	}
	bool controller::connected() {
		return this->is_connected;
	}
	input_manager::device_type controller::get_type() {
		return input_manager::device_type::controller;
	}
	controller::joysticks controller::get_joysticks() {
		return this->js;
	}
	const DIDATAFORMAT* controller::get_format() {
		return &c_dfDIJoystick;
	}
	void controller::device_specific_init() {
		DIDEVCAPS capabilities;
		capabilities.dwSize = sizeof(DIDEVCAPS);
		this->device->GetCapabilities(&capabilities);
		this->buttons = capabilities.dwButtons;
		memset(&this->js, 0, sizeof(joysticks));
		this->current.resize(this->buttons);
		this->last.resize(this->buttons);
		for (size_t i = 0; i < this->buttons; i++) {
			this->last[i] = false;
		}
		ZeroMemory(&this->state, sizeof(DIJOYSTATE));
		debug::log_print("created controller");
	}
}