#pragma once
class gameobject;
class component {
public:
	class property_base {
	public:
		property_base(const std::string& name, size_t size);
		const std::string& get_name();
		virtual ~property_base();
		void* get_ptr();
		virtual void draw() const = 0;
	protected:
		std::string name;
		void* ptr;
	};
	using properties_t = std::vector<std::unique_ptr<property_base>>;
	template<typename T> class property : public property_base {
	public:
		property(const T& value, const std::string& name);
		property(const property<T>& other);
		const property<T>& operator=(const property<T>& other);
		virtual ~property() override;
		virtual void draw() const override;
		T* get_value();
	private:
		T* value;
	};
	component(gameobject* obj);
	virtual void initialize();
	virtual void pre_update();
	virtual void post_update();
	virtual void pre_render();
	virtual void post_render();
	virtual void clean_up();
	const properties_t& get_properties() const;
protected:
	void add_property(property_base* p);
	template<typename _Ty> property<_Ty>* find_property(const std::string& name);
	properties_t properties;
	gameobject* parent;
};
class test_component : public component {
public:
	test_component(gameobject* obj);
	void print_properties();
};
class debug_component : public component {
public:
	debug_component(gameobject* obj);
	virtual void pre_render() override;
private:
	double flash_start_time, flash_end_time;
};
template<typename T> inline component::property<T>::property(const T& value, const std::string& name) : property_base(name, sizeof(T)) {
	this->value = new(this->ptr) T(value);
}
template<typename T> inline component::property<T>::property(const property<T>& other) : property(*other.value, other.name) { }
template<typename T> inline const component::property<T>& component::property<T>::operator=(const property<T>& other) {
	*this->value = *other.value;
	this->name = other.name;
}
template<typename T> inline component::property<T>::~property() {
	this->value->~T();
	free(this->ptr);
}
template<typename T> inline T* component::property<T>::get_value() {
	return this->value;
}
template<typename T> inline void component::property<T>::draw() const {
	ImGui::Text("sorry, no implementation for this type yet... heres the raw memory though");
	ImGui::InputText(this->name.c_str(), (char*)this->ptr, sizeof(T));
}
template<typename _Ty> inline component::property<_Ty>* component::find_property(const std::string& name) {
	property<_Ty>* result = NULL;
	for (auto& p : this->properties) {
		if (p->get_name() == name) {
			result = (property<_Ty>*)p.get();
			break;
		}
	}
	return result;
}
