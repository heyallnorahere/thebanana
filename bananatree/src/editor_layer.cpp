#define BANANA_MAIN
#include <thebanana.h>
#include "../resource.h"
namespace bananatree {
	class editor_layer : public thebanana::application_layer {
	public:
		virtual void init() override;
		virtual void gameloop() override;
	};
}
thebanana::application_layer* create_application_layer() {
	return new bananatree::editor_layer;
}
namespace bananatree {
	void editor_layer::init() {
		thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT));
		thebanana::g_game->get_scene()->set_shader_name("basic");
		thebanana::g_game->load_models();
	}
	void editor_layer::gameloop() {
		thebanana::g_game->update();
		thebanana::g_game->render();
	}
}