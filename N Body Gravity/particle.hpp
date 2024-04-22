#pragma once

#include "Vec2f.hpp"
#include <vector>

using namespace std;

class particle
{
public:
    Vec2f nPos{ 0,0 };
    float lastAcc = 0;
    
    //Leapfrog Integration
    Vec2f vel{ 0,0 };
    Vec2f nAcc{ 0,0 };
    Vec2f oAcc{ 0,0 };

    //Simple Verlet
    Vec2f oPos{ 0,0 };
    Vec2f acc{ 0,0 };

    int gSpace = 0;
    float radius = 5;
    float mass = 1;
    float pressure = 0;
    int index;
    vector<float> particleColor = { 255,0,0,32 };

    void simpleVerletUpdatePos();

    void leapFrogUpdatePos();

    void boundary(float wallElasticity);

    void accelerate(Vec2f acceleration);

    void getColorAcc(float maxAcc);

    void draw();

    Vec2f gravitateParticle(particle* other);

    Vec2f gravitateCell(Vec2f otherPos, float otherMass);
};