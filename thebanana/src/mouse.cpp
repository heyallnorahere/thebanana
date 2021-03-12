#include "pch.h"
#include "mouse.h"
#include "input_utils.h"
#include "debug_tools.h"
#include "game.h"
namespace thebanana {
	mouse::mouse() : is_connected(true) { }
	mouse::~mouse() { }
	std::vector<mouse::button> mouse::get_buttons() {
		return this->current;
	}
	void mouse::update() {
		if (platform_specific::get_focus() == this->parent->get_parent()->get_window().m && this->parent->get_parent()->should_clip_cursor()) {
#ifdef BANANA_WINDOWS
			RECT clip_rect;
			GetClientRect(this->parent->get_parent()->get_window().m, &clip_rect);
			ClientToScreen(this->parent->get_parent()->get_window().m, (POINT*)& clip_rect.left);
			ClientToScreen(this->parent->get_parent()->get_window().m, (POINT*)& clip_rect.right);
			ClipCursor(&clip_rect);
#endif
		} else {
#ifdef BANANA_WINDOWS
			ClipCursor(NULL);
#endif
		}
#ifdef BANANA_WINDOWS
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
#endif
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
#ifdef BANANA_WINDOWS
		return &c_dfDIMouse;
#else
		return NULL;
#endif
	}
	void mouse::device_specific_init() {
#ifdef BANANA_WINDOWS
		this->buttons = 4;
		memset(&this->pos, 0, sizeof(glm::vec2));
		this->current.resize(this->buttons);
		this->last.resize(this->buttons);
		for (size_t i = 0; i < this->buttons; i++) {
			this->last[i] = false;
		}
		ZeroMemory(&this->state, sizeof(DIMOUSESTATE));
		this->parent->get_parent()->debug_print("created mouse");
#endif
	}
}