//
// Created by Nikita Krutoy on 08.07.2021.
//



#ifndef GAME_GAMEOBJECT_H
#define GAME_GAMEOBJECT_H


#include <iostream>
#include "Rasterizer.h"
#include "Geometry.h"
#include "GameState.h"
#include "Resources.h"




struct GameObject {
    GameObject() = default;

    Point position = {0, 0};
    float scale = 1;
    Point forwardDirection = {1, 0};
    bool enabled = true;

    std::shared_ptr<Rasterizer> r = nullptr;

    virtual void setRasterizer(std::shared_ptr<Rasterizer> r) {
        this->r = r;
    };
    void SafeDraw() {
        if(!enabled) return;
        if (r) {
            Draw();
        } else {
            std::cout << "No Rasterizer" << std::endl;
        }
    }

    void SafeUpdate(float dt) {
        if(!enabled) return;
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
    size_t size;
    Color c;
    size_t spacing;
    bool align;

    TextObject() = default;

    TextObject(const std::string &text, Point p, size_t size,
               const Color &c, size_t spacing, bool align) :   text(text),
                                                               size(size), c(c),
                                                               spacing(spacing),
                                                               align(align) {position = p;}

    TextObject(std::string text): text(text) {};
    void Draw() override {
        r->drawText(text, position, size, c, spacing, align);
    }
};

struct LivesText: TextObject{
    LivesText() =default;
    LivesText(const std::string &text, Point p, size_t size, const Color &c,
               size_t spacing, bool align) : TextObject(text, p, size, c, spacing, align) {};
    void Update(float dt) override {text = "Lives: " + std::to_string(GameState::lives);}
};
struct ScoreText: TextObject{
    ScoreText() =default;
    ScoreText(const std::string &text, Point p, size_t size, const Color &c,
              size_t spacing, bool align) : TextObject(text, p, size, c, spacing, align) {};
    void Update(float dt) override {text = "Score: " + std::to_string(GameState::score);}
};

struct SpaceObject: GameObject {
    Point velocity = {0, 0};
    float speed = 1;
    Path path;
    bool isSeamless = true;
    Color c = Color();


    SpaceObject() : path({}) {};

    explicit SpaceObject(Path path) : path(path) {};

    void Boost(float amount);
    virtual void Rotate(float degree);
protected:

    void Draw() override;
    void Update(float dt) override;
};

struct Spaceship: SpaceObject {
    Spaceship() = default;

    void enableInvincibility() {
        isInvincible = true;
        elapsedTime = 0;
    }
    explicit Spaceship(Path p): SpaceObject(p) {};
    void disableInvincibility() {
        isInvincible = false;
        c = Color();
    }

protected:
    bool isInvincible = false;
    float elapsedTime = 0;
    void Update(float dt) override;
};

struct Projectile: SpaceObject {
    explicit Projectile(Path &path) : SpaceObject(path) {
        isSeamless = false;
    };
protected:
    void Draw() override {
        r->drawBlob(position, 1);
    }
};

struct Asteroid: SpaceObject {
    float radius = 5;

    explicit Asteroid(const Path &path) : SpaceObject(path) {};

    explicit Asteroid(float radius, Point position, float speed, bool isSeamless);
};

struct Billboard: GameObject {
    unsigned char* buf;
    int width;
    int height;
    int comp;
    Billboard() = default;

    Billboard(unsigned char* b, size_t size);

protected:
    void Draw() override {
        r->drawImage(buf, width, height, position);
    }
};

struct BackgroundObjcet : SpaceObject {
    int size;
    float elapsedTime = 0;
    float maxBrightness = 0.1;
    float minBrightness = 0.01;
    float blinkSpeed = 0.5;
    float offset = 0;

    void Update(float dt) override{
        elapsedTime += dt;
        float br = (0.5f * sinf((elapsedTime * blinkSpeed + offset) * 1) + 0.5f) * (maxBrightness - minBrightness) + minBrightness;
        c = Color(br, br, br);
        SpaceObject::Update(dt);
    }
};


struct Planet: BackgroundObjcet {
protected:
    void Draw() override{
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
};

struct Star: BackgroundObjcet {
    float fsize;
protected:
    void Draw() override{
        int i1,i2;
        r->enableSeamless();
        for (int x = -fsize; x < fsize + 1; x++) {
            if (x == 0)
                i1 = fsize + 1;
            else
                i1 = std::floor(abs(1 / float(x) * size)) * sgn(x);
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

protected:
    void Update(float dt) override {
        fsize = size + 10 *  (0.5 * sinf((elapsedTime * blinkSpeed + offset)) + 0.5);
        BackgroundObjcet::Update(dt);
    }
};

#endif //GAME_GAMEOBJECT_H
