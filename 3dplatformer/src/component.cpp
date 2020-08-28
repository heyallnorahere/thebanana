#include "pch.h"
#include "component.h"
#include "gameobject.h"
component::component(gameobject* obj) : parent(obj) { }
test_component::component_id test_component::get_component_id() {
	return 0xff;
}
test_component::test_component(gameobject* obj) : component(obj) { }
void test_component::initialize() {
	OutputDebugString(TEXT("test component initialized\n"));
}
void test_component::update() {
	OutputDebugString(TEXT("test component updated\n"));
}
void test_component::clean_up() {
	OutputDebugString(TEXT("test component uninitialized\n"));
}
void test_component::do_stuff() {
	throw "owo";
}
