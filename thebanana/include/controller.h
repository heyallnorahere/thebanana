#pragma once
#include "input_manager.h"
#include "banana_api.h"
namespace thebanana {
	class controller : public input_manager::device {
	public:
		struct joysticks {
			glm::vec2 left, right;
		};
		BANANA_API controller();
		BANANA_API ~controller();
		BANANA_API virtual std::vector<button> get_buttons() override;
		BANANA_API virtual void update() override;
		BANANA_API virtual bool connected() override;
		BANANA_API virtual input_manager::device_type get_type() override;
		BANANA_API joysticks get_joysticks();
	protected:
		BANANA_API virtual const DIDATAFORMAT* get_format() override;
		BANANA_API virtual void device_specific_init() override;
	private:
		joysticks js;
		state_t<DIJOYSTATE> state;
		std::vector<button> current;
		std::vector<bool> last;
		bool is_connected;
		size_t buttons;
	};
}