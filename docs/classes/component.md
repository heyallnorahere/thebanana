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
[<- Back](basic_gameobject.md)

[<- Back to index](../README.md)