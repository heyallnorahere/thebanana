#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	class gameobject;
	class material;
	class mesh_component : public component {
	public:
		BANANA_API mesh_component(gameobject* obj);
		BANANA_API mesh_component& set_mesh_name(const std::string& name);
		BANANA_API std::string get_mesh_name() const;
		BANANA_API material* get_material();
		BANANA_API virtual void pre_render() override;
		BANANA_API virtual void render() override;
	private:
		std::string model_name;
	};
}