#define BANANA_MAIN
#include <thebanana.h>
#include "editor_layer.h"
thebanana::application_layer* create_application_layer() {
	return new bananatree::editor_layer;
}