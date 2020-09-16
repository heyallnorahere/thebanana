#pragma once
#include "component.h"
class gameobject;
class mesh_component : public component {
public:
	mesh_component(gameobject* obj);
	mesh_component& set_mesh_name(const std::string& name);
	std::string get_mesh_name() const;
	virtual void pre_render() override;
	void render();
private:
	std::string model_name;
};