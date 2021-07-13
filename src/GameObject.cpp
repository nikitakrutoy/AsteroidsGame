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

void GameObject::SafeDraw() {
    if(!(enabled && doDraw)) return;
    if (r) {
        Draw();
    } else {
        std::cout << "No Rasterizer" << std::endl;
    }
}

void GameObject::SafeUpdate(float dt) {
    if(!(enabled && doUpdate)) return;
    if (r) {
        Update(dt);
    } else {
        std::cout << "No Rasterizer" << std::endl;
    }
}

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



Asteroid::Asteroid(float radius, Point p, float s, bool seamless) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<float>  dist3(0, 2 * 3.14);
    position = p;
    speed = s;
    isSeamless = seamless;
    std::uniform_int_distribution<int>  distr(radius - 3, radius + 3);
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

Billboard::Billboard(unsigned char *b, size_t size) {
    buf = stbi_load_from_memory(b, size, &width, &height, &comp, STBI_grey_alpha);
}

void BackgroundObject::Update(float dt) {
    elapsedTime += dt;
    float br = (0.5f * sinf((elapsedTime * blinkSpeed + offset) * 1) + 0.5f) * (maxBrightness - minBrightness) + minBrightness;
    c = Color(br, br, br);
    SpaceObject::Update(dt);
}

void Planet::Draw() {
    int i1,i2;
    r->enableSeamless();
    for (int x = -size; x < size + 1; x++) {
        i1 = size * sin(acos(float(x) / size));
        i2 = -i1;
        if (i2 < i1) std::swap(i1, i2);
        for (int y = i1; y < i2 + 1; y++) {
            r->setPixel(position.x + x, position.y + y, c);
        }
    }
    r->disableSeamless();
}


void Star::Draw() {
    int i1,i2;
    r->enableSeamless();
    for (int x = -fsize; x < fsize + 1; x++) {
        if (x == 0)
            i1 = fsize + 1;
        else
            i1 = std::floor(std::abs(1 / float(x) * size)) * sgn(x);
        i2 = -i1;
        if (i2 < i1) std::swap(i1, i2);
        for (int y = i1; y < i2 + 1; y++) {
            r->setPixel(position.x + x, position.y + y, c);
        }
    }
    r->setPixel(position.x - size -1, position.y, c);
    r->setPixel(position.x + size + 1, position.y, c);
    r->disableSeamless();
}
#endif //GAME_GAMEOBJECT_CPP





