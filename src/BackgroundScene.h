//
// Created by Nikita Krutoy on 12.07.2021.
//

#ifndef ASTEROIDS_BACKGROUNDSCENE_H
#define ASTEROIDS_BACKGROUNDSCENE_H

#include <random>
#include "Scene.h"
#include "GameObject.h"

struct BackgroundScene: Scene {
    std::vector<Star> stars;
    std::vector<Planet> planets;

    BackgroundScene() = default;
    void Init() override{
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<float>  dist2(0, 1);
//        std::uniform_int_distribution<float>  dist3(3, 5);
        for (int i = 0; i < 15; i++) {
            Star star = Star();
            star.size = dist2(generator) * 25 + 15;
            star.position = Point(
                    dist2(generator) * r->width,
                    dist2(generator) * r->height);
            star.velocity = Point(
                    0.05 * ((2 * dist2(generator) - 1)),
                    0.05 * ((2 * dist2(generator) - 1))
                    );
            star.offset = dist2(generator) * 1000;
            star.setRasterizer(r);
            stars.push_back(star);
        }
        for (int i = 0; i < 25; i++) {
            Planet planet = Planet();
            planet.size = dist2(generator) * 34 + 1;
            planet.position = Point(
                    dist2(generator) * r->width,
                    dist2(generator) * r->height);
            planet.velocity = Point(
                    0.05 * ((2 * dist2(generator) - 1)),
                    0.05 * ((2 * dist2(generator) - 1))
            );
            planet.offset = dist2(generator) * 1000;
            planet.setRasterizer(r);
            planets.push_back(planet);
        }
    }
protected:
    void Draw() override {
        r->fillColor();
        for (auto &p : planets) p.SafeDraw();
        for (auto &s : stars) s.SafeDraw();
    }
    void Update(float dt) override {
        for (auto &p : planets) p.SafeUpdate(dt);
        for (auto &s : stars) s.SafeUpdate(dt);
    }
};


#endif //ASTEROIDS_BACKGROUNDSCENE_H
