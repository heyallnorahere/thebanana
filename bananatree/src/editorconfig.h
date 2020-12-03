#pragma once
namespace bananatree {
	class editorconfig {
	public:
		// will override another property if it exists with the same key
		template<typename T> void set(const std::string& key, const T& value);
		// USE WISELY - will throw an exception if you cast
		template<typename T> const T& get(const std::string& key);
		void load(const std::string& path);
		void save(const std::string& path);
		void restore_defaults();
		~editorconfig();
	private:
		class configproperty_base {
		public:
			virtual size_t get_type_hash() = 0;
			virtual ~configproperty_base();
		};
		template<typename T> class configproperty : public configproperty_base {
		public:
			configproperty(const T& value);
			T* get_pointer();
			virtual size_t get_type_hash() override;
			virtual ~configproperty() override;
		private:
			T* value;
		};
		void clear_map();
		std::map<std::string, configproperty_base*> m_properties;
	};
	template<typename T> inline void editorconfig::set(const std::string& key, const T& value) {
		// get the property from the map (if it doesnt exist, it returns null)
		auto& p = this->m_properties[key];
		if (!p) {
			// if the property does not exist, create a new one
			this->m_properties[key] = new configproperty<T>(value);
		} else {
			// otherwise...
			if (p->get_type_hash() != typeid(T).hash_code()) {
				// if the property is of a different type, delete it and create another
				delete p;
				this->m_properties[key] = new configproperty<T>(value);
			} else {
				// or, just set the value on an existing one
				*(((configproperty<T>*)this->m_properties[key])->get_pointer()) = value;
			}
		}
	}
	template<typename T> inline const T& editorconfig::get(const std::string& key) {
		// get the property from the map
		auto& p = this->m_properties[key];
		// make sure the user isnt casting
		assert(typeid(T).hash_code() == p->get_type_hash());
		// get the pointer from the property
		configproperty<T>* prop = (configproperty<T>*)p;
		return *prop->get_pointer();
	}
	template<typename T> inline editorconfig::configproperty<T>::configproperty(const T& value) {
		this->value = new T(value);
	}
	template<typename T> inline T* editorconfig::configproperty<T>::get_pointer() {
		return this->value;
	}
	template<typename T> inline size_t editorconfig::configproperty<T>::get_type_hash() {
		return typeid(T).hash_code();
	}
	template<typename T> inline editorconfig::configproperty<T>::~configproperty() {
		delete this->value;
	}
}