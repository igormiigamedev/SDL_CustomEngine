#pragma once

#include <SDL.h>
#include "Vector2D.h"

class Transform {

public:
    Transform(float x = 0.0f, float y = 0.0f, float rotation=0.0f) {

        X = x;
        Y = y;
        Rotation = rotation;
    }
    void Log(std::string msg = "") {
        std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << std::endl;
    }

private:
    inline void TranslateX(float x) { X += x; }
    inline void TranslateY(float y) { Y += y; }
    inline void Translate(Vector2D v) { X += v.X; Y += v.Y; }

public:
    float X, Y;
    float Rotation;

};
