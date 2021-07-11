//
// Created by Nikita Krutoy on 08.07.2021.
//


#ifndef GAME_GAMEOBJECT_H
#define GAME_GAMEOBJECT_H

#include <iostream>
#include "Rasterizer.h"
#include "Geometry.h"
#include "GameState.h"


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


struct WaveBackground: GameObject {
    float elapsedTime = 0;
    const int   complexity      = 10;    // More points of color.
    const float fluid_speed     = 60.0;  // Drives speed, higher number will make it slower.
    const float color_intensity = 0.5;
    const float Pi = 3.14159;

    WaveBackground() = default;

    WaveBackground(const WaveBackground& bg) : GameObject(bg) {};

protected:
    void Update(float dt) override {
        elapsedTime += dt;
    };

    void Draw() override {
        float x,y, w, h, v, d;
        float s = float(std::max(r->width, r->height));
        w = r->width;
        h = r->height;
        for (int i = 0; i < r->width; i++) {
            for (int j = 0; j < r->height; j++) {
                x = float(i) / w;
                y = float(j) / h;
                d = Distance(Point(x, y), Point(0.5, 0.5));
                v = customSin(y * h / 8) * customSin(x * w / 8) * customSin(y + elapsedTime);
                Color c = Color(v, v, v);
                r->setPixel(i, j, c);
            }
        }
    }

private:
    inline float customSin(float x) {return 0.5 * sin(x) + 0.5;};
};


#endif //GAME_GAMEOBJECT_H
