# The Banana
This is a 3D game engine I'm making. It currently supports the following platforms:
- Windows

## TO IMPLEMENT
If you want to implement WinMain/main on your own, define `BANANA_MAIN_HANDLED` before including `thebanana.h`. It is required to set up the window (`WNDCLASS`, etc.) if you choose this option.

Otherwise, your main file should look something like this:
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