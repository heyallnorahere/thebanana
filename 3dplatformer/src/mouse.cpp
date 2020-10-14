#include "pch.h"
#include "mouse.h"
#include "input_utils.h"
#include "debug_tools.h"
namespace thebanana {
	mouse::mouse() : is_connected(true) { }
	mouse::~mouse() { }
	std::vector<mouse::button> mouse::get_buttons() {
		return std::vector<button>();
	}
	void mouse::update() {
		this->is_connected = SUCCEEDED(this->device->Poll());
		HRESULT hr = this->device->GetDeviceState(sizeof(DIMOUSESTATE), &this->state);
		if (FAILED(hr)) __debugbreak();
		for (size_t i = 0; i < this->buttons; i++) {
			this->last[i] = this->current[i].held;
		}
		calc_pressed(this->current, this->state, this->buttons);
		calc_down(this->current, this->last, this->buttons);
		calc_up(this->current, this->last, this->buttons);
		float x = dinput_mouse_axis(this->state.val.lX);
		float y = -dinput_mouse_axis(this->state.val.lY);
		this->pos += glm::vec2(x, y);
	}
	bool mouse::connected() {
		return this->is_connected;
	}
	input_manager::device_type mouse::get_type() {
		return input_manager::device_type::mouse;
	}
	glm::vec2 mouse::get_pos() {
		return this->pos;
	}
	const DIDATAFORMAT* mouse::get_format() {
		return &c_dfDIMouse;
	}
	void mouse::device_specific_init() {
		this->buttons = 4;
		memset(&this->pos, 0, sizeof(glm::vec2));
		this->current.resize(this->buttons);
		this->last.resize(this->buttons);
		for (size_t i = 0; i < this->buttons; i++) {
			this->last[i] = false;
		}
		ZeroMemory(&this->state, sizeof(DIMOUSESTATE));
		log_print("created mouse");
	}
}