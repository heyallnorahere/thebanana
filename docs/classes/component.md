# Component
Components are what define the behavior of each object every frame.
## Basic use
```cpp
// im using mesh_component for demonstration purposes
using example_component = thebanana::mesh_component;
// adds a component to an object (returns that component)
my_gameobject->add_component<example_component>();
// gets the first instance of a component on an object
example_component& first = my_gameobject->get_component<example_component>();
// gets the component at the specified index
example_component& at_index = my_gameobject->get_component<example_component>(0);
// gets the number of components of the specified type (size_t)
size_t count = my_gameobject->get_number_components<example_component>();
// returns whether or not there is a component of the specified type
bool has = my_gameobject->has_component<example_component>();
```
## Functions
- `void initialize()`

This function is called once either the [Gameobject](gameobject.md) is added to the scene, or the Component is added to the [Gameobject](gameobject.md).

The following three update functions are called when the parent [Gameobject](gameobject.md)'s `update()` function is called.

- `void pre_update()`

This function is called before `update()` is called.
- `void update()`

This function is called after `pre_update()`, but before `post_update()`.
- `void post_update()`

This function is called after `update()` is called.

The following three render functions are called when the parent [Gameobject](gameobject.md)'s `render()` function is called.

- `void pre_render()`

This function is called before `render()` is called.
- `void render()`

This function is called after `pre_render()`, but before `post_render()`.
- `void post_render()`

This function is called after `render()` is called.
- `void on_collision(gameobject* other)`

This function is called once the parent [Gameobject](gameobject.md) collides with another object.
- `void clean_up()`

This function is called once the parent [Gameobject](gameobject.md)'s destructor is called.

[<- Back](basic_gameobject.md) [Next ->](game.md)

[<- Back to index](../README.md)