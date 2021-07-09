//
// Created by Nikita Krutoy on 08.07.2021.
//


#ifndef GAME_GAMEOBJECT_H
#define GAME_GAMEOBJECT_H

#include <iostream>
#include "Rasterizer.h"
#include "Geometry.h"


struct GameObject {
    GameObject() = default;

    Point position = {0, 0};
    float scale = 1;
    Point forwardDirection = {1, 0};

    std::shared_ptr<Rasterizer> r = nullptr;

    virtual void setRasterizer(std::shared_ptr<Rasterizer> r) {
        this->r = r;
    };
    void SafeDraw() {
        if (r) {
            Draw();
        } else {
            std::cout << "No Rasterizer" << std::endl;
        }
    }

    void SafeUpdate(float dt) {
        if (r) {
            Update(dt);
        } else {
            std::cout << "No Rasterizer" << std::endl;
        }
    }

    virtual void Delete() {
        r.reset();
    }

protected:
    virtual void Draw() {};
    virtual void Update(float dt) {};
};

struct TextObject: GameObject {
    std::string text;
    void Draw() override {
        r->drawText(text, position);
    }
};

struct SpaceObject: GameObject {
    Point velocity = {0, 0};
    float speed = 1;
    Path path;

    SpaceObject() : path({}) {};

    explicit SpaceObject(Path path) : path(std::move(path)) {};

    void Boost(float amount);
    virtual void Rotate(float degree);
protected:
    void Draw() override;
    void Update(float dt) override;
};

struct Projectile: SpaceObject {
    explicit Projectile(Path &path) : SpaceObject(path) {};

protected:
    void Draw() override {
        r->drawBlob(position, 1);
    }
    void Update(float dt) override{
        position = position.Translate(velocity.Scale(speed));
    }
};

struct Asteroid: SpaceObject {
    float radius = 5;

    explicit Asteroid(const Path &path) : SpaceObject(path) {};

    explicit Asteroid(float radius, size_t quantity = 20);
};

#endif //GAME_GAMEOBJECT_H
