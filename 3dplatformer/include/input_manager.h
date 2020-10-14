#pragma once
#include "util.h"
namespace thebanana {
	class game;
	class input_manager {
	public:
		struct dinput_callback_env {
			double time;
			input_manager* im;
		};
		struct dinput_device {
			const DIDEVICEINSTANCE inst;
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
			bool init(const dinput_device& device, IDirectInput8* context);
			virtual ~device();
			virtual std::vector<button> get_buttons() = 0;
			virtual void update() = 0;
			virtual bool connected() = 0;
			virtual device_type get_type() = 0;
		protected:
			virtual const DIDATAFORMAT* get_format() = 0;
			virtual void device_specific_init() = 0;
			IDirectInputDevice8* device;
		};
		input_manager(game* g_game);
		void enum_devices();
		std::vector<dinput_device> get_enumerated_devices();
		~input_manager();
		device_type add_device(size_t index);
		device_type add_device(const win32_string& name);
		void update_devices();
		size_t get_num_devices();
		std::vector<device::button> get_device_buttons(size_t index);
		static device_type get_device_type(const dinput_device& dev);
		device_type get_device_type(size_t index);
		device* get_device(size_t index);
	private:
		game* m_game;
		IDirectInput8* m_context;
		dinput_callback_env* m_env;
		static int __stdcall enum_callback(const DIDEVICEINSTANCE* inst, dinput_callback_env* passed_env);
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