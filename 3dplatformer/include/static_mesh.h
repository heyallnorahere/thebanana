#pragma once
#include "gameobject.h"
namespace thebanana {
	class static_mesh : public gameobject {
	public:
		static_mesh(const std::string& model_name);
		virtual void update() override;
		virtual void render() override;
		virtual ~static_mesh() override;
	};
}