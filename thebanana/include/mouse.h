#pragma once
#include "input_manager.h"
namespace thebanana {
	class mouse : public input_manager::device {
	public:
		mouse();
		~mouse();
		virtual std::vector<button> get_buttons() override;
		virtual void update() override;
		virtual bool connected() override;
		virtual input_manager::device_type get_type() override;
		glm::vec2 get_pos();
	protected:
		virtual const DIDATAFORMAT* get_format() override;
		virtual void device_specific_init() override;
	private:
		bool is_connected;
		std::vector<button> current;
		std::vector<bool> last;
		state_t<DIMOUSESTATE> state;
		glm::vec2 pos;
		size_t buttons;
	};
}