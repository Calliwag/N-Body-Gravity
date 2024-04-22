#pragma once

#include <vector>
#include <chrono>
#include "Vec2f.hpp"
#include "particle.hpp"
#include "QuadTree.hpp"

using namespace std;

class sim
{
public:
    vector<particle> P;
    int size = 800;
    int qTreeCapacity = 1;
    float wallElasticity = 1;
    float minMass = 1;
    float maxMass = 1;
    float maxAcc = 0;
    int frames = 0;
    Vec2f cameraOffset = { 0,0 };
    float renderScale = .5;

    sim() = default;

    void update();

    void boundary();

    void updatePos();

    void draw();

    void particleGravity();

    void particleGravityMultiThread(int threadCount);

    static vector<particle> particleGravityBatch(int index1, int index2, QuadTree* q, vector<particle>* P);
};