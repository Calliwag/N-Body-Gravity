#include <iostream>
#include "resources.hpp"
#include "Vec2f.hpp"
#include "particle.hpp"
#include "sim.hpp"
#include "QuadTree.hpp"

using namespace std;
using Vec2f = glm::vec2;

int main()
{
    /*ChangeDirectory(GetApplicationDirectory());
    ChangeDirectory("C:\\Output");
    cout << GetApplicationDirectory << " " << GetWorkingDirectory << endl;*/

    SetTargetFPS(75);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    sim s;
    s.size = max(window.GetWidth(), window.GetHeight());
    s.wallElasticity = 0;
    s.minMass = 1;
    s.maxMass = 1;
    s.qTreeCapacity = 1;

    float startRadius = 500;
    float particleNumber = 10000;
    float totalMass = particleNumber * 0.5 * (s.minMass + s.maxMass);

    for (int i = 0; i < particleNumber; i++)
    {
        particle newP;
        newP.index = i;
        newP.nPos = { GetRandomValue(-startRadius,startRadius),GetRandomValue(-startRadius,startRadius) };
        while (length(newP.nPos) > startRadius)
        {
            newP.nPos = { GetRandomValue(-startRadius,startRadius),GetRandomValue(-startRadius,startRadius) };
        }
        newP.mass = (float)randomDouble(s.minMass, s.maxMass);
        newP.radius = 25;
        newP.particleColor = { 255,0,0,32 };
        Vec2f v = { -newP.nPos.y, newP.nPos.x };
        v = v / length(v) * sqrt(totalMass * length(newP.nPos) / (startRadius * startRadius));
        newP.vel = v;
        newP.oPos = newP.nPos - v;
        s.P.push_back(newP);
    }

    /*particle newP;
    newP.index = s.P.size();
    newP.nPos = { 0,0 };
    newP.mass = 10000;
    newP.radius = 25;
    Vec2f v = { 0,0 };
    newP.oPos = newP.nPos - v;
    newP.particleColor = {255,255,255,127};
    s.P.push_back(newP);*/

    s.renderScale = .2;

    raylib::Camera2D camera;
    camera.target = { s.cameraOffset.x,s.cameraOffset.y };
    camera.offset = { 400,400 };
    camera.zoom = s.renderScale;
    camera.rotation = 0;
    bool running = 1;

    while (!window.ShouldClose()) {
        BeginDrawing();
        camera.BeginMode();
        window.ClearBackground(BLACK);
        if (running)
        {
            s.update();
        }
        if (IsKeyDown(KEY_UP)) s.cameraOffset.y -= 5 / s.renderScale;
        if (IsKeyDown(KEY_DOWN)) s.cameraOffset.y += 5 / s.renderScale;
        if (IsKeyDown(KEY_LEFT)) s.cameraOffset.x -= 5 / s.renderScale;
        if (IsKeyDown(KEY_RIGHT)) s.cameraOffset.x += 5 / s.renderScale;
        if (IsKeyPressed(KEY_SPACE)) running = 1- running;
        if (GetMouseWheelMove() == -1)
        {
            s.renderScale *= .8;
        }
        if (GetMouseWheelMove() == 1)
        {
            s.renderScale *= 1.25;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            particle newP;
            newP.index = s.P.size();
            newP.nPos = { GetScreenToWorld2D(GetMousePosition(), camera).x ,GetScreenToWorld2D(GetMousePosition(), camera).y };
            newP.mass = 100;
            newP.radius = 10;
            Vec2f v = { 0,0 };
            newP.oPos = newP.nPos - v;
            newP.particleColor = { 255,255,255,127 };
            s.P.push_back(newP);
        }
        camera.zoom = s.renderScale;
        camera.target = { s.cameraOffset.x,s.cameraOffset.y };
        s.draw();
        camera.EndMode();
        raylib::DrawText("Frames: " + to_string(s.frames)
            + "\nFrame Time: " + to_string(window.GetFrameTime()) + "s"
            + "\nQuadTree Capacity: " + to_string(s.qTreeCapacity)
            + "\nRender Center: " + to_string(s.cameraOffset.x) + " , " + to_string(s.cameraOffset.y)
            + "\nRender Scale: " + to_string(s.renderScale), 0, 0, 18, WHITE);
        EndDrawing();
        string name = "frame" + to_string(s.frames) + ".png";
        //TakeScreenshot(name.c_str());
        if (s.frames > 1000)
        {
            break;
        }
    }

    return 0;
}