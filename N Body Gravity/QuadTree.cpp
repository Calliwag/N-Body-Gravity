#include "QuadTree.hpp"

QuadTree::QuadTree(rect newDomain)
{
	domain = newDomain;
}

bool QuadTree::insert(particle* Point)
{
	if (!domain.containsPoint(Point))
	{
		return false;
	}

	if (points.size() < capacity && children.size() == 0)
	{
		points.push_back(Point);
		return true;
	}

	if (children.size() == 0)
	{
		points.push_back(Point);
		subDivide();
		for (int i = 0; i < points.size(); i++)
		{
			if (children[0].insert(points[i]))
			{
				continue;
			}
			if (children[1].insert(points[i]))
			{
				continue;
			}
			if (children[2].insert(points[i]))
			{
				continue;
			}
			if (children[3].insert(points[i]))
			{
				continue;
			}
		}
		return true;
	}

	if (children.size() != 0)
	{
		points.push_back(Point);
		children[0].insert(Point);
		children[1].insert(Point);
		children[2].insert(Point);
		children[3].insert(Point);
		return true;
	}

	return false;
}

void QuadTree::subDivide()
{
	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			rect r({ domain.center.x + (float)i * domain.halfDim / 2, domain.center.y + (float)j * domain.halfDim / 2 }, (float)domain.halfDim / 2);
			QuadTree q(r);
			q.capacity = capacity;
			children.push_back(q);
		}
	}
}

void QuadTree::getMasses()
{
	for (int i = 0; i < points.size(); i++)
	{
		mass += points[i]->mass;
		centerOfMass += points[i]->nPos * points[i]->mass;
	}
	centerOfMass = centerOfMass * (1 / mass);

	if (children.size() > 0)
	{
		children[0].getMasses();
		children[1].getMasses();
		children[2].getMasses();
		children[3].getMasses();
	}
}

Vec2f QuadTree::getGravity(particle* Point)
{
	Vec2f acceleration = {};

	if (children.size() == 0)
	{
		for (int i = 0; i < points.size(); i++)
		{
			acceleration += Point->gravitateParticle(points[i]);
		}
		return acceleration;
	}

	if (2 * domain.halfDim / length(Point->nPos - centerOfMass) < distanceRatio)
	{
		acceleration += Point->gravitateCell(centerOfMass, mass);
		return acceleration;
	}

	acceleration += children[0].getGravity(Point);
	acceleration += children[1].getGravity(Point);
	acceleration += children[2].getGravity(Point);
	acceleration += children[3].getGravity(Point);
}

void QuadTree::draw()
{
	domain.draw({ 255,255,255,255 });
	if (children.size() > 0)
	{
		children[0].draw();
		children[1].draw();
		children[2].draw();
		children[3].draw();
	}
}

vector<int> QuadTree::getPointsInRange(rect Range)
{
	vector<int> pointsInRange = {};

	if (children.size() == 0)
	{
		for (int i = 0; i < points.size(); i++)
		{
			if (Range.containsPoint(points[i]))
			{
				pointsInRange.push_back(points[i]->index);
			}
		}
		return pointsInRange;
	}

	if (children[0].domain.intersectsRect(Range))
	{
		vector<int> pIR0 = children[0].getPointsInRange(Range);
		pointsInRange.insert(pointsInRange.end(), pIR0.begin(), pIR0.end());
	}

	if (children[1].domain.intersectsRect(Range))
	{
		vector<int> pIR1 = children[0].getPointsInRange(Range);
		pointsInRange.insert(pointsInRange.end(), pIR1.begin(), pIR1.end());
	}
	if (children[2].domain.intersectsRect(Range))
	{
		vector<int> pIR2 = children[2].getPointsInRange(Range);
		pointsInRange.insert(pointsInRange.end(), pIR2.begin(), pIR2.end());
	}
	if (children[3].domain.intersectsRect(Range))
	{
		vector<int> pIR3 = children[3].getPointsInRange(Range);
		pointsInRange.insert(pointsInRange.end(), pIR3.begin(), pIR3.end());
	}

	return pointsInRange;
}

void QuadTree::erase()
{
	points = {};
	children = {};
}
