#pragma once

class Circle
{
public:
    Circle(float X = 0, float Y = 0, float Radius=0) : x(X), y(Y), radius(Radius) {}
    float x, y; // Center of the circle
    float radius; // radius of the circle
};

