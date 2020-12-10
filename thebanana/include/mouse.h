#pragma once
#include "input_manager.h"
#include "banana_api.h"
namespace thebanana {
	class mouse : public input_manager::device {
	public:
		BANANA_API mouse();
		BANANA_API ~mouse();
		BANANA_API virtual std::vector<button> get_buttons() override;
		BANANA_API virtual void update() override;
		BANANA_API virtual bool connected() override;
		BANANA_API virtual input_manager::device_type get_type() override;
		BANANA_API glm::vec2 get_pos();
	protected:
		BANANA_API virtual const DIDATAFORMAT* get_format() override;
		BANANA_API virtual void device_specific_init() override;
	private:
		bool is_connected;
		std::vector<button> current;
		std::vector<bool> last;
		state_t<DIMOUSESTATE> state;
		glm::vec2 pos;
		size_t buttons;
	};
}