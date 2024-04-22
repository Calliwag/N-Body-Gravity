#pragma once

#include "Vec2f.hpp"
#include "raylib-cpp.hpp"
#include "particle.hpp"

class rect
{
public:
	Vec2f center = { 0,0 };
	float halfDim = 0;
	
	rect(Vec2f newCenter, float newHalfDim);
	rect() = default;
	bool containsPoint(particle* Point);
	bool intersectsRect(rect other);
	void draw(vector<int> C);
};