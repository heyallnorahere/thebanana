#include "pch.h"
#include "components/animation_component.h"
#include "components/mesh_component.h"
#include "util.h"
#include "gameobject.h"
#include "game.h"
#include "internal_util.h"
namespace thebanana {
	animation_component::animation_component(gameobject* obj) : component(obj), animation_start_time(0.0), repeat(false), animation_id(-1) { }
	void animation_component::post_update() {
		if (this->animation_id > -1) {
			double current_time = this->get_animation_time();
			std::string model_name = "";
			if (this->get_number_components<mesh_component>() > 0) {
				model_name = this->get_component<mesh_component>().get_mesh_name();
			}
			const aiScene* s = this->parent->get_game()->get_model_registry()->get_scene(model_name);
			if (!s) return;
			aiAnimation* animation = s->mAnimations[this->animation_id];
			double length_in_sec = animation->mDuration / animation->mTicksPerSecond;
			if (!this->repeat && current_time > length_in_sec) {
				this->stop_animation();
			}
		}
	}
	void animation_component::start_animation(int index, bool repeat) {
		this->animation_id = index;
		this->animation_start_time = CURRENT_TIME(double);
		this->repeat = repeat;
	}
	void animation_component::start_animation(const std::string& name, bool repeat) {
		std::string model_name = "";
		if (this->get_number_components<mesh_component>() > 0) {
			model_name = this->get_component<mesh_component>().get_mesh_name();
		}
		const aiScene* scene = this->parent->get_game()->get_model_registry()->get_scene(model_name);
		if (!scene) return;
		for (int i = 0; i < scene->mNumAnimations; i++) {
			std::string animation_name(scene->mAnimations[i]->mName.data, scene->mAnimations[i]->mName.length);
			if (name == animation_name.substr(animation_name.find_last_of('|') + 1)) {
				this->start_animation(i, repeat);
				break;
			}
		}
	}
	void animation_component::stop_animation() {
		this->animation_id = -1;
		this->repeat = false;
	}
	double animation_component::get_animation_time() {
		return CURRENT_TIME(double) - this->animation_start_time;
	}
	int animation_component::get_animation_id() const {
		return this->animation_id;
	}
	bool animation_component::is_animating() const {
		return this->animation_id != -1;
	}
}