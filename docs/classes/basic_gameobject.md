# Basic Gameobject
## Basic Use
```cpp
thebanana::gameobject* obj = new thebanana::basic_gameobject;
// add components and children, edit transform, etc.
thebanana::g_game->get_scene()->add_object(obj);
```
## Functions
This class is just a simple implementation of a [Gameobject](gameobject.md), and is used in the [Scene Serializer](scene_serializer.md).

[<- Back](application_layer.md) [Next ->](component.md)

[<- Back to index](../README.md)