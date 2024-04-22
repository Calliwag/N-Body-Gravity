#include "Rect.hpp"
#include <iostream>

rect::rect(Vec2f newCenter, float newHalfDim)
{
    center = newCenter;
    halfDim = newHalfDim;
}

bool rect::containsPoint(particle* point)
{
    if (point->nPos.x < center.x + halfDim && point->nPos.x >= center.x - halfDim &&
        point->nPos.y < center.y + halfDim && point->nPos.y >= center.y - halfDim)
    {
        return true;
    }
    return false;
}

bool rect::intersectsRect(rect other)
{
    float selfXMax = glm::max(center.x + halfDim, center.x - halfDim);
    float selfXMin = glm::min(center.x + halfDim, center.x - halfDim);
    float selfYMax = glm::max(center.y + halfDim, center.y - halfDim);
    float selfYMin = glm::min(center.y + halfDim, center.y - halfDim);

    float otherXMax = glm::max(other.center.x + other.halfDim, other.center.x - other.halfDim);
    float otherXMin = glm::min(other.center.x + other.halfDim, other.center.x - other.halfDim);
    float otherYMax = glm::max(other.center.y + other.halfDim, other.center.y - other.halfDim);
    float otherYMin = glm::min(other.center.y + other.halfDim, other.center.y - other.halfDim);

    bool overlapX = 0;
    bool overlapY = 0;

    if (selfXMin <= otherXMax && selfXMax >= otherXMin)
    {
        overlapX = 1;
    }

    if (selfYMin <= otherYMax && selfYMax >= otherYMin)
    {
        overlapY = 1;
    }

    return(overlapX && overlapY);
}

void rect::draw(vector<int> C)
{
    Color Color;
    Color.r = C[0];
    Color.g = C[1];
    Color.b = C[2];
    Color.a = C[3];
    DrawRectangleLines(center.x - halfDim, center.y - halfDim, 2 * halfDim, 2 * halfDim, Color);
}
