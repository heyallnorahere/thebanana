#pragma once
#include "gameobject.h"
namespace thebanana {
	class prop : public gameobject {
	public:
		prop(const std::string& model_name);
		virtual void update() override;
		virtual void render() override;
		virtual ~prop() override;
	};
}