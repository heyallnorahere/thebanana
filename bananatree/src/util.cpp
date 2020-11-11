#include <thebanana.h>
#include "util.h"
void* load_file_from_resource(int resource, const std::string& type, size_t& size) {
	HRSRC resource_ = FindResourceA(NULL, MAKEINTRESOURCEA(resource), type.c_str());
	if (!resource_) return NULL;
	HGLOBAL global = LoadResource(NULL, resource_);
	if (!global) return NULL;
	void* data = LockResource(global);
	if (!data) return NULL;
	size = (size_t)SizeofResource(NULL, resource_);
	if (!size) return NULL;
	return data;
}
