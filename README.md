# The Banana
This is a 3D game engine I'm making. It currently supports the following platforms:
- Windows

## TO IMPLEMENT
If you want to implement WinMain/main on your own, **do not** define `BANANA_MAIN` before including `thebanana.h` in your `main.cpp`. It is required to set up the window (`WNDCLASS`, etc.) if you choose this option.

Otherwise, your `main.cpp` file should look something like this:
```cpp
// defining this tells thebanana.h to define WinMain/main
#define BANANA_MAIN
#include <thebanana.h>

void init_game() {
    // initialize thebanana::g_game, load models, add gameobjects to the scene, etc.
}
void gameloop() {
    // update and render thebanana::g_game
}
void clean_up_game() {
    // delete thebanana::g_game, unload game-specific resources, etc.
}
```

For all methods of implementation, `thebanana::g_game` *needs* to be initialized and freed for the engine to properly work.

## TO BUILD
### WINDOWS
[Visual Studio 2019](https://visualstudio.microsoft.com/) solution/project included, with NuGet packages installed.
The only thing you need to download is the [Steamworks SDK](https://partner.steamgames.com/) and copy the `sdk` folder into `C:/steamsdk`. Then, copy `redistributable_bin/win64/steam_api64.dll` to the main project directory.