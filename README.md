# The Banana
This is a 3D game engine I'm making. (the repository is named "3dplatformer" because I didnt initially have a name for the engine) Many aspects of this engine are inspired by The Cherno's [Hazel](https://github.com/TheCherno/Hazel). It currently supports the following platforms:
- Windows

## TO IMPLEMENT
If you want to implement WinMain/main on your own, **do not** define `BANANA_MAIN` before including `thebanana.h` in your `main.cpp`. It is required to set up the window (`WNDCLASS`, etc.) and initialize `thebanana::g_game` if you choose this option.

Otherwise, your `main.cpp` file should look something like this:
```cpp
// defining this tells thebanana.h to define WinMain/main
#define BANANA_MAIN
#include <thebanana.h>

// this class defines the games behavior and whatnot
class my_game_application_layer : public thebanana::application_layer {
public:
	// all of these are optional,
	// though if you dont define any of them
	// (or dont define this class at all and use the base class),
	// the "game" is just gonna be a tech demo
	virtual void init() override;
	virtual void gameloop() override;
	virtual void clean_up() override;
	virtual std::string window_title() override;
}

// this tells thebanana.h which application layer class to use
thebanana::application_layer* create_application_layer() {
	return new my_game_application_layer;
}

void my_game_application_layer::init() {
	// initialize your game (add gameobjects, load models, etc.)
}
void my_game_application_layer::gameloop() {
	// it's advised that you either dont define this or use the following lines of code:
	thebanana::g_game->update();
	thebanana::g_game->render();
}
void my_game_application_layer::clean_up() {
	// clean up your game (free resources and memory)
}
std::string my_game_application_layer::window_title() {
	// this method decides what title your game's window will have
	return "my game";
}
```

## TO BUILD
### WINDOWS
[Visual Studio 2019](https://visualstudio.microsoft.com/) solution/project included, with NuGet packages installed.
The only thing you need to download is the [Steamworks SDK](https://partner.steamgames.com/) and copy the `sdk` folder into `C:/steamsdk`. Then, copy `redistributable_bin/win64/steam_api64.dll` to the sandbox project directory.

If you've done this, you're good to go!