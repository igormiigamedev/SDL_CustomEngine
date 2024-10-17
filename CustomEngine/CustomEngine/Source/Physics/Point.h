#pragma once
#include <iostream>

class Point{
    public:
        float X, Y;

    public:
        Point(float x = 0, float y = 0) : X(x), Y(y) {}

    public:
        // addition +
        inline Point operator+(const Point& p2) const {
            return Point(X + p2.X, Y + p2.Y);
        }

        // addition +=
        inline friend Point operator+=(Point& p1, const Point& p2){
            p1.X += p2.X;
            p1.Y += p2.Y;
            return p1;
        }

        // difference -
        inline Point operator-(const Point& p2) const {
            return Point(X - p2.X, Y - p2.Y);
        }

        // difference -=
        inline friend Point operator-=(Point& p1, const Point& p2) {
            p1.X -= p2.X;
            p1.Y -= p2.Y;
            return p1;
        }


        // multiplication *
        inline Point operator*(const float scalar) const {
            return Point(X * scalar, Y * scalar);
        }

        void Log(std::string msg = "") {
            std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << std::endl;
        }
};

