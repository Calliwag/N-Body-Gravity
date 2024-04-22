#include "particle.hpp"
#include "resources.hpp"

void particle::simpleVerletUpdatePos()
{
    Vec2f tempVel = nPos - oPos;

    oPos = nPos;

    nPos += tempVel + acc;

    lastAcc = length(acc);
    acc = {};
}

void particle::leapFrogUpdatePos()
{
    oPos = nPos;
    nPos += vel + 0.5f * oAcc;
    vel += 0.5f * (nAcc + oAcc);
    oAcc = nAcc;
    nAcc = {};
}

void particle::boundary(float wallElasticity)
{
    Vec2f vel = nPos - oPos;

    if (nPos.x < radius)
    {
        nPos.x = radius;
        vel.x *= -wallElasticity;
    }
    else if (nPos.x > window.GetWidth() - radius)
    {
        nPos.x = window.GetWidth() - radius;
        vel.x *= -wallElasticity;
    }

    if (nPos.y < radius)
    {
        nPos.y = radius;
        vel.y *= -wallElasticity;
    }
    else if (nPos.y > window.GetHeight() - radius)
    {
        nPos.y = window.GetHeight() - radius;
        vel.y *= -wallElasticity;
    }

    oPos = nPos - vel;
}

void particle::accelerate(Vec2f acceleration)
{
    acc += acceleration;
    nAcc += acceleration;
}

void particle::getColorAcc(float maxAcc)
{
    float ratio = min(1.0f,(maxAcc - lastAcc) / maxAcc);
    particleColor = {255,255 * (1 - ratio),0,particleColor[3]};
}

void particle::draw()
{
    Color Col;
    Col.r = particleColor[0];
    Col.g = particleColor[1];
    Col.b = particleColor[2];
    Col.a = particleColor[3];
    Color Col2;
    Col2.r = particleColor[0];
    Col2.g = particleColor[1];
    Col2.b = particleColor[2];
    Col2.a = 0;
    //DrawCircle(nPos.x, nPos.y, radius, Col);
    DrawPixel(nPos.x, nPos.y, Col);
    DrawCircleGradient(nPos.x, nPos.y, radius, Col, Col2);
}

Vec2f particle::gravitateParticle(particle* other)
{
    if (index == other->index)
    {
        return { 0,0 };
    }
    Vec2f deltaPos = nPos - other->nPos;
    float inverseDist = 1.0f / vecLength(deltaPos);
    Vec2f axis = inverseDist * deltaPos;
    float accMag = other->mass / (vecLengthSq(deltaPos) + (1.0 / sqrt(other->mass)) + other->mass);
    Vec2f acceleration = -axis * accMag;
    return acceleration;
}

Vec2f particle::gravitateCell(Vec2f otherPos, float otherMass)
{
    Vec2f deltaPos = nPos - otherPos;
    float inverseDist = 1.0f / vecLength(deltaPos);
    Vec2f axis = inverseDist * deltaPos;
    float accMag = otherMass / (vecLengthSq(deltaPos) + (1.0 / sqrt(otherMass)) + otherMass);
    Vec2f acceleration = -axis * accMag;
    return acceleration;
}
