#pragma once
#include "basic_gameobject.h"
#include "banana_api.h"
namespace thebanana {
	class static_mesh : public basic_gameobject {
	public:
		BANANA_API static_mesh(const std::string& model_name);
		BANANA_API virtual ~static_mesh() override;
	};
}