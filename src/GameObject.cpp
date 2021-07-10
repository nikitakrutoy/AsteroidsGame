//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMEOBJECT_CPP
#define GAME_GAMEOBJECT_CPP

#include <random>
#include <utility>
#include "GameObject.h"

void SpaceObject::Update(float dt) {
    position = position.Translate(velocity.Scale(speed));
    float empty = 0;
    position = Point(
            int(r->width + std::floor(position.x)) % r->width + std::modf(position.x, &empty),
            int(r->height + std::floor(position.y)) % r->height + std::modf(position.x, &empty)
    );
}

void SpaceObject::Boost(float amount) {
    velocity = velocity.Translate(forwardDirection.Scale(amount));
}

void SpaceObject::Rotate(float degree) {
    path = path.Rotate(degree);
    forwardDirection = forwardDirection.Rotate(degree);
}

void SpaceObject::Draw()  {
    Path p = path.Scale(scale).Translate(position);
    r->drawPath(p);
}

void Spaceship::Draw()  {
    Path p = path.Scale(scale).Translate(position);
    r->drawPath(p, c);
}

void Spaceship::Update(float dt) {
    SpaceObject::Update(dt);
    if (isInvincible) {
        float v = std::round((std::sin(elapsedTime * 20) + 1));
        std::cout << elapsedTime << std::endl;
        c = Color() * v;
        elapsedTime += dt;
    }
}


std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_real_distribution<float>  dist3(0, 2 * 3.14);

Asteroid::Asteroid(float radius, size_t quantity) {
    std::uniform_int_distribution<float>  distr(radius - 3, radius + 3);
    std::vector<Point> data;
    data.reserve(quantity);
    for (int i = 0; i < quantity; i++) {

        float radian = 360 / 20 * i * (M_PI/180);
        data.emplace_back(
                std::cos(radian) * distr(generator),
                std::sin(radian) * distr(generator)
        );
    }
    path = Path(data);
    float angle = dist3(generator);
    velocity = Point(
            std::cos(angle),
            std::sin(angle)
    ).Scale(1);
    this->radius = radius;
}

#endif //GAME_GAMEOBJECT_CPP
