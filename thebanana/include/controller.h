#pragma once
#include "input_manager.h"
namespace thebanana {
	class controller : public input_manager::device {
	public:
		struct joysticks {
			glm::vec2 left, right;
		};
		controller();
		~controller();
		virtual std::vector<button> get_buttons() override;
		virtual void update() override;
		virtual bool connected() override;
		virtual input_manager::device_type get_type() override;
		joysticks get_joysticks();
	protected:
		virtual const DIDATAFORMAT* get_format() override;
		virtual void device_specific_init() override;
	private:
		joysticks js;
		state_t<DIJOYSTATE> state;
		std::vector<button> current;
		std::vector<bool> last;
		bool is_connected;
		size_t buttons;
	};
}