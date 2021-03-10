#pragma once
#include "util.h"
#include "keys.h"
#include "banana_api.h"
namespace thebanana {
	class game;
	class input_manager {
	public:
		struct dinput_callback_env {
			double time;
			input_manager* im;
		};
		struct dinput_device {
			const DIDEVICEINSTANCEA inst;
			dinput_callback_env env;
		};
		enum class device_type {
			other,
			controller,
			mouse,
			keyboard,
			does_not_exist,
		};
		class device {
		public:
			struct button {
				bool down;
				bool up;
				bool held;
			};
			template<typename T> struct state_t {
				T val;
				unsigned char operator[](size_t index) const;
			};
			BANANA_API bool init(const dinput_device& device, IDirectInput8A* context, input_manager* parent);
			BANANA_API virtual ~device();
			virtual std::vector<button> get_buttons() = 0;
			virtual void update() = 0;
			virtual bool connected() = 0;
			virtual device_type get_type() = 0;
		protected:
			virtual const DIDATAFORMAT* get_format() = 0;
			virtual void device_specific_init() = 0;
			IDirectInputDevice8A* device;
			input_manager* parent;
		};
		BANANA_API input_manager(game* g_game);
		BANANA_API void enum_devices();
		BANANA_API std::vector<dinput_device> get_enumerated_devices();
		BANANA_API ~input_manager();
		BANANA_API device_type add_device(size_t index);
		BANANA_API device_type add_device(const std::string& name);
		BANANA_API void update_devices();
		BANANA_API size_t get_num_devices();
		BANANA_API std::vector<device::button> get_device_buttons(size_t index);
		BANANA_API static device_type get_device_type(const dinput_device& dev);
		BANANA_API device_type get_device_type(size_t index);
		BANANA_API device* get_device(size_t index);
		BANANA_API game* get_parent();
		BANANA_API device::button get_key(key k);
		BANANA_API device::button get_mouse_button(mouse_button mb);
		BANANA_API size_t find_device(device_type type);
	private:
		game* m_game;
		IDirectInput8A* m_context;
		dinput_callback_env* m_env;
		BANANA_API static int __stdcall enum_callback(const DIDEVICEINSTANCEA* inst, dinput_callback_env* passed_env);
		std::vector<dinput_device> enumerated_devices;
		std::list<std::unique_ptr<device>> devices;
	};
	inline unsigned char input_manager::device::state_t<DIJOYSTATE>::operator[](size_t index) const {
		return this->val.rgbButtons[index];
	}
	inline unsigned char input_manager::device::state_t<DIMOUSESTATE>::operator[](size_t index) const {
		return this->val.rgbButtons[index];
	}
	template<typename T> inline unsigned char input_manager::device::state_t<T>::operator[](size_t index) const {
		return static_cast<unsigned char>(this->val[index]);
	}
}