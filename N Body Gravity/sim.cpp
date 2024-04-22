#include "sim.hpp"
#include "QuadTree.hpp"
#include "resources.hpp"
#include "raylib-cpp.hpp"
#include <iostream>
#include <thread>
#include <future>

void sim::update()
{
    frames++;
    particleGravityMultiThread(4);
    //particleGravity();
    updatePos();
    //boundary();
}

void sim::boundary()
{
    for (int i = 0; i < P.size(); i++)
    {
        P[i].boundary(wallElasticity);
    }
}

void sim::updatePos()
{
    for (int i = 0; i < P.size(); i++)
    {
        //P[i].leapFrogUpdatePos();
        P[i].simpleVerletUpdatePos();
    }
}

void sim::draw()
{
    cout << maxAcc << endl;
    float newMaxAcc = 0;
    for (int i = 0; i < P.size(); i++)
    {
        P[i].getColorAcc(maxAcc / 2);
        P[i].draw();
        newMaxAcc = max(newMaxAcc, P[i].lastAcc);
    }
    maxAcc = newMaxAcc;
}

void sim::particleGravity()
{
    Vec2f mins = { P[0].nPos.x,P[0].nPos.y };
    Vec2f maxs = { P[0].nPos.x,P[0].nPos.y };

    for (int i = 1; i < P.size(); i++)
    {
        if (P[i].nPos.x < mins.x)
        {
            mins.x = P[i].nPos.x;
        }
        else if (P[i].nPos.x > maxs.x)
        {
            maxs.x = P[i].nPos.x;
        }
        if (P[i].nPos.y < mins.y)
        {
            mins.y = P[i].nPos.y;
        }
        else if (P[i].nPos.y > maxs.y)
        {
            maxs.y = P[i].nPos.y;
        }
    }

    float size = max(maxs.x - mins.x, maxs.y - mins.y) / 2;
    Vec2f center = mins + Vec2f{ size, size };

    QuadTree q({ center, size + 1 });
    q.capacity = qTreeCapacity;
    for (int i = 0; i < P.size(); i++)
    {
        q.insert(&P[i]);
    }
    q.getMasses();
    q.draw();

    for (int i = 0; i < P.size(); i++)
    {
        P[i].accelerate(q.getGravity(&P[i]));
    }
}

void sim::particleGravityMultiThread(int threadCount)
{
    Vec2f mins = { P[0].nPos.x,P[0].nPos.y };
    Vec2f maxs = { P[0].nPos.x,P[0].nPos.y };

    for (int i = 1; i < P.size(); i++)
    {
        if (P[i].nPos.x < mins.x)
        {
            mins.x = P[i].nPos.x;
        }
        else if (P[i].nPos.x > maxs.x)
        {
            maxs.x = P[i].nPos.x;
        }
        if (P[i].nPos.y < mins.y)
        {
            mins.y = P[i].nPos.y;
        }
        else if (P[i].nPos.y > maxs.y)
        {
            maxs.y = P[i].nPos.y;
        }
    }

    float size = max(maxs.x - mins.x, maxs.y - mins.y) / 2;
    Vec2f center = mins + Vec2f{ size, size };

    QuadTree q({ center, size + 1 });
    q.capacity = qTreeCapacity;
    for (int i = 0; i < P.size(); i++)
    {
        q.insert(&P[i]);
    }
    q.getMasses();

    vector<future<vector<particle>>> batches (threadCount);
    for (int i = 0; i < threadCount; i++)
    {
        batches[i] = async(&particleGravityBatch, i * P.size() / threadCount, (i + 1) * P.size() / threadCount, &q, &P);
    }
    for (int i = 0; i < threadCount; i++)
    {
        vector<particle> batch = batches[i].get();
        for (int j = 0; j < batch.size(); j++)
        {
            P[i * P.size() / threadCount + j] = batch[j];
        }
    }
}

vector<particle> sim::particleGravityBatch(int index1, int index2, QuadTree* q, vector<particle>* P)
{
    vector<particle> output (index2 - index1);
    for (int i = index1; i < index2; i++)
    {
        output[i - index1] = P->at(i);
        output[i-index1].accelerate(q->getGravity(&P->at(i)));
    }
    return output;
}
