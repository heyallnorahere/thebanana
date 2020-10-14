#include "pch.h"
#include "null_object.h"
null_object::null_object() {
	this->m_nickname = "null object";
}
void null_object::update() {
	this->prepare_for_update();
	this->post_update();
}
void null_object::render() {
	this->prepare_for_render();
	this->post_render();
}