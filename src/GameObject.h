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
#include <memory>




struct GameObject {
    GameObject() = default;

    Point position = {0, 0};
    float scale = 1;
    Point forwardDirection = {1, 0};
    bool enabled = true;
    bool doUpdate = true;
    bool doDraw = true;

    std::shared_ptr<Rasterizer> r = nullptr;

    virtual void SetRasterizer(std::shared_ptr<Rasterizer> r) {
        this->r = r;
    };
    void SafeDraw() ;
    void SafeUpdate(float dt);

    virtual void Delete() {
        r.reset();
    }

protected:
    virtual void Draw() {};
    virtual void Update(float dt) {};
};

struct TextObject: GameObject {
    std::string text;
    float size;
    Color c = Color();
    size_t spacing;
    float rotation = 0;
    bool align;

    TextObject() = default;

    TextObject(const std::string &text, Point p, float size,
               const Color &c, size_t spacing, bool align) :   text(text),
                                                               size(size), c(c),
                                                               spacing(spacing),
                                                               align(align) {position = p;}

    TextObject(std::string text): text(text) {};
    void Draw() override {
        r->DrawText(text, position, size, rotation, c, spacing, align);
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
        r->DrawBlob(position, 1);
    }
};

struct Asteroid: SpaceObject {
    float radius = 5;

    explicit Asteroid(const Path &path) : SpaceObject(path) {};

    explicit Asteroid(float radius, Point position, float speed, bool isSeamless);
};

struct Billboard: GameObject {
    const unsigned char* buf;
    int width;
    int height;
    int comp;
    Billboard() = default;

    Billboard(unsigned char* b, size_t size);

protected:
    void Draw() override {
        r->DrawImage(buf, width, height, position);
    }
};

struct BackgroundObject : SpaceObject {
    int size;
    float elapsedTime = 0;
    float maxBrightness = 0.1;
    float minBrightness = 0.01;
    float blinkSpeed = 0.5;
    float offset = 0;
    Color c = Color(0,0,0,0);

protected:
    void Update(float dt) override;
};


struct Planet: BackgroundObject {
protected:
    void Draw() override;
};

struct Star: BackgroundObject {
    float fsize;
protected:
    void Draw() override;

protected:
    void Update(float dt) override {
        fsize = size + 10 *  (0.5 * sinf((elapsedTime * blinkSpeed + offset)) + 0.5);
        BackgroundObject::Update(dt);
    }
};

#endif //GAME_GAMEOBJECT_H
