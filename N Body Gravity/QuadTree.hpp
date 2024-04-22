#pragma once

#include "Rect.hpp"
#include <vector>
#include "particle.hpp"

class QuadTree
{
public:
	int capacity = 1;
	rect domain = { {400,400},400 };

	float mass = 0;
	Vec2f centerOfMass = {};
	float distanceRatio = 1;

	vector<particle*> points = {};
	vector<QuadTree> children = {};

	QuadTree(rect newDomain);
	bool insert(particle* Point);
	void subDivide();
	void getMasses();
	Vec2f getGravity(particle* Point);
	void draw();
	vector<int> getPointsInRange(rect Range);
	void erase();
};