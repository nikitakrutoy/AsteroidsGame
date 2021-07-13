//
// Created by Nikita Krutoy on 12.07.2021.
//

#include "BackgroundScene.h"
#include "MagicNumbers.h"

void SpaceBackgroundScene::Init(){
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<float>  dist2(0, 1);
    stars.clear();
    planets.clear();
    for (int i = 0; i < STARS_COUNT; i++) {
        Star star = Star();
        star.size = dist2(generator) * 25 + 15;
        star.position = Point(
                dist2(generator) * r->width,
                dist2(generator) * r->height);
        star.velocity = Point(
                BACKGROUND_VELOCITY * ((2 * dist2(generator) - 1)),
                BACKGROUND_VELOCITY * ((2 * dist2(generator) - 1))
        );
        star.offset = dist2(generator) * 1000;
        star.SetRasterizer(r);
        star.minBrightness = minBrightness;
        star.maxBrightness = maxBrightness;
        stars.push_back(star);
    }
    for (int i = 0; i < PLANETS_COUNT; i++) {
        Planet planet = Planet();
        planet.size = dist2(generator) * 34 + 1;
        planet.position = Point(
                dist2(generator) * r->width,
                dist2(generator) * r->height);
        planet.velocity = Point(
                BACKGROUND_VELOCITY * ((2 * dist2(generator) - 1)),
                BACKGROUND_VELOCITY * ((2 * dist2(generator) - 1))
        );
        planet.offset = dist2(generator) * 1000;
        planet.SetRasterizer(r);
        planet.minBrightness = minBrightness;
        planet.maxBrightness = maxBrightness;
        planets.push_back(planet);
    }
}