#pragma once
#include "basic_gameobject.h"
namespace thebanana {
	class static_mesh : public basic_gameobject {
	public:
		static_mesh(const std::string& model_name);
		virtual ~static_mesh() override;
	};
}