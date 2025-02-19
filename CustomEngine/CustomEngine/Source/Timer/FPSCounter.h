#pragma once
#include <iostream>
#include <chrono>

class FPSCounter {
public:
    FPSCounter() : frameCount(0), lastTime(std::chrono::high_resolution_clock::now()) {}

    void Update() {
        frameCount++;
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = now - lastTime;

        if (duration.count() >= 1.0f) {
            fps = frameCount;
            frameCount = 0;
            lastTime = now;
            std::cout << "FPS: " << fps << std::endl;
        }
    }

private:
    int frameCount;
    int fps;
    std::chrono::high_resolution_clock::time_point lastTime;
};

