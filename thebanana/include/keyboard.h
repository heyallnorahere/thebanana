#pragma once
#include "input_manager.h"
#include "banana_api.h"
namespace thebanana {
	class keyboard : public input_manager::device {
	public:
		BANANA_API keyboard();
		BANANA_API ~keyboard();
		BANANA_API virtual std::vector<button> get_buttons() override;
		BANANA_API virtual void update() override;
		BANANA_API virtual bool connected() override;
		BANANA_API virtual input_manager::device_type get_type() override;
	protected:
		BANANA_API virtual const DIDATAFORMAT* get_format() override;
		BANANA_API virtual void device_specific_init() override;
	private:
		size_t keys;
		state_t<unsigned char[256]> state;
		bool is_connected;
		std::vector<button> current;
		std::vector<bool> last;
	};
}