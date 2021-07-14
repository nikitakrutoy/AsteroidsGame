//
// Created by Nikita Krutoy on 12.07.2021.
//

#ifndef ASTEROIDS_BACKGROUNDSCENE_H
#define ASTEROIDS_BACKGROUNDSCENE_H

#include <random>
#include "Scene.h"
#include "GameObject.h"

struct SolidBackgroundScene: Scene {
    Color c = Color(0,0,0, 1);
protected:
    void Draw() override {
        r->FillColor(c);
    }
};


struct SpaceBackgroundScene: Scene {
public:
    SpaceBackgroundScene() = default;
    SpaceBackgroundScene(float minBrightness, float maxBrightness):
        minBrightness(minBrightness), maxBrightness(maxBrightness){};
    void Init() override;
private:
    std::vector<Star> stars;
    std::vector<Planet> planets;
    const float minBrightness = 0.01, maxBrightness = 0.1;

protected:
    void Draw() override {
        r->FillColor();
        for (auto &p : planets) p.SafeDraw();
        for (auto &s : stars) s.SafeDraw();
    }
    void Update(float dt) override {
        for (auto &p : planets) p.SafeUpdate(dt);
        for (auto &s : stars) s.SafeUpdate(dt);
    }
};


#endif //ASTEROIDS_BACKGROUNDSCENE_H
