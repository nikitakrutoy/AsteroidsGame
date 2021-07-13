//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMEOBJECT_CPP
#define GAME_GAMEOBJECT_CPP

#include <random>
#include "Geometry.h"
#include "GameObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void SpaceObject::Update(float dt) {
    position = position.Translate(velocity.Scale(speed));
    if (isSeamless)
        position = Point(
                frame(position.x, r->width),
                frame(position.y, r->height)
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
    if (isSeamless) {
        r->enableSeamless();
        r->drawPath(p, c);
        r->disableSeamless();
    } else {
        r->drawPath(p, c);
    }

}

void Spaceship::Update(float dt) {
    SpaceObject::Update(dt);
    if (isInvincible) {
        float v = std::round((std::sin(elapsedTime * 20) + 1));
        c = Color() * v;
        elapsedTime += dt;
    }
}


std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_real_distribution<float>  dist3(0, 2 * 3.14);

Asteroid::Asteroid(float radius, Point p, float s, bool seamless) {
    position = p;
    speed = s;
    isSeamless = seamless;
    std::uniform_int_distribution<float>  distr(radius - 3, radius + 3);
    std::vector<Point> data;
    data.reserve(20);
    for (int i = 0; i < 20; i++) {

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

Billboard::Billboard(unsigned char *b, size_t size) {
    buf = stbi_load_from_memory(b, size, &width, &height, &comp, STBI_grey_alpha);
}
