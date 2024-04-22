#pragma once
#include "raylib-cpp.hpp"
#define vecLength(vec) sqrt(vec.x * vec.x + vec.y * vec.y)
#define vecLengthSq(vec) vec.x * vec.x + vec.y * vec.y

extern raylib::Window window;
double randomDouble(double min, double max);