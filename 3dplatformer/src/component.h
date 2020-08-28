#pragma once
class gameobject;
class component {
public:
	using component_id = unsigned long long;
	component(gameobject* obj);
	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void clean_up() = 0;
protected:
	gameobject* parent;
};
class test_component : public component {
public:
	static component_id get_component_id();
	test_component(gameobject* obj);
	virtual void initialize() override;
	virtual void update() override;
	virtual void clean_up() override;
	void do_stuff();
};