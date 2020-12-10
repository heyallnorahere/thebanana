#pragma once
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define DIRECTINPUT_VERSION 0x800
#include <Windows.h>
#include <initguid.h>
#include <dinput.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifdef interface
#undef interface
#endif
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <chrono>
#include <cassert>
#include <typeinfo>
#include <thread>
#include <limits>