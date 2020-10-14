#pragma once
#define dinput_mouse_axis(val) (static_cast<float>(val) / static_cast<float>(std::numeric_limits<char>::max()))
#define dinput_controller_axis(val) (static_cast<float>(val) / static_cast<float>(std::numeric_limits<unsigned short>::max()))
namespace thebanana {
	inline bool calc_pressed(unsigned char val) {
		const unsigned char pressed = 0b10000000;
		return val & pressed;
	}
	template<typename T> inline void calc_pressed(std::vector<input_manager::device::button>& btns, const input_manager::device::state_t<T>& state, size_t buttons) {
		for (size_t i = 0; i < buttons; i++) {
			btns[i].held = calc_pressed(state[i]);
		}
	}
	inline bool calc_down(bool current, bool last) {
		return (current) && (!last);
	}
	inline void calc_down(std::vector<input_manager::device::button>& current, const std::vector<bool>& last, size_t buttons) {
		for (size_t i = 0; i < buttons; i++) {
			current[i].down = calc_down(current[i].held, last[i]);
		}
	}
	inline bool calc_up(bool current, bool last) {
		return (!current) && (last);
	}
	inline void calc_up(std::vector<input_manager::device::button>& current, const std::vector<bool>& last, size_t buttons) {
		for (size_t i = 0; i < buttons; i++) {
			current[i].up = calc_up(current[i].held, last[i]);
		}
	}
}