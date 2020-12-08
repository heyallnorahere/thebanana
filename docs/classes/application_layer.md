# Application Layer
## Basic use
```cpp
class my_application_layer : public thebanana::application_layer {
public:
	virtual void register_scripts() override;
	virtual void init() override;
	virtual void gameloop() override;
	virtual void clean_up() override;
	virtual std::string window_title() override;
private:
	// for an example later
	thebanana::script_module* m_module;
}
```
## Functions
- `void register_scripts()`

This function should be used to register scripts, either from a [Script Module](script_module.md), or from the executable itself. For example:
```cpp
void my_application_layer::register_scripts() {
	// from the executable
	thebanana::g_game->get_script_registry()->register_script<my_script>();
	// from a script module:
	std::string path = "path/to/script/module.dll"
	// let the engine handle your module... (you may only load one module at a time)
	thebanana::g_game->load_script_module(path);
	// ...or manage one yourself
	this->m_module =
		new thebanana::script_module(thebanana::g_game->get_script_registry(), path);
}
```

- `void init()`

This function should load models and shaders, add [Gameobjects](gameobject.md) to the scene, and load sound effects. Ex:
```cpp
void my_application_layer::init() {
	// for (working directory)/shaders/basic/(type of shader).shader
	thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::file_shader("basic"));
	// add a gameobject with a mesh component and rigidbody
	thebanana::g_game->get_scene()->add_object(new thebanana::static_mesh("cube"));
	// load a model
	thebanana::model_registry::model_descriptor desc;
	desc.name = "cube";
	desc.path = "path/to/cube.obj";
	desc.texture_proc = my_texture_function; // more on this later
	thebanana::g_game->add_model_desc(desc);
	thebanana::g_game->load_models();
}
```

- `void gameloop()`

This function decides what order things happen in during each frame. Ex:
```cpp
void my_application_layer::gameloop() {
	// note: i strongly recommend not defining this or using the provided code:
	thebanana::g_game->update();
	thebanana::g_game->render();
	thebanana::g_game->swap_buffers();
}
```

- `void clean_up()`

This function frees memory and resources that have been allocated. Ex:
```cpp
void my_application_layer::clean_up() {
	// the script module from earlier
	delete this->m_module;
}
```

- `std::string window_title()`

This one is simple. It tells the engine what to assign the windows title to. Ex:
```cpp
std::string my_application_layer::window_title() {
	return "Documentation example app";
}
```
[Next ->](basic_gameobject.md)

[<- Back to index](../README.md)