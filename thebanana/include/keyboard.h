#pragma once
#include "input_manager.h"
namespace thebanana {
	class keyboard : public input_manager::device {
	public:
		keyboard();
		~keyboard();
		virtual std::vector<button> get_buttons() override;
		virtual void update() override;
		virtual bool connected() override;
		virtual input_manager::device_type get_type() override;
	protected:
		virtual const DIDATAFORMAT* get_format() override;
		virtual void device_specific_init() override;
	private:
		size_t keys;
		state_t<unsigned char[256]> state;
		bool is_connected;
		std::vector<button> current;
		std::vector<bool> last;
	};
}