//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMEFIELDSCENE_H
#define GAME_GAMEFIELDSCENE_H

#include "Scene.h"

struct GameFieldScene: Scene {
private:
    void InitAsteroids(size_t quantity);
    bool wasSpacePressed = false;
    bool isInfinite = false;
    size_t quantity = 10;
    size_t lives = 3;
    time_t invTime = 3;
    time_t invTimer = 0;

    Spaceship player{};
    std::vector<Asteroid> asteroids{};
    std::vector<Projectile> projectiles{};
    ScoreText scoreText{};
    LivesText livesText{};
    Point startPoint{};
protected:
    void Draw() override;

public:
    GameFieldScene() = default;
    GameFieldScene(size_t quantity, bool isInfinite,
                   size_t lives = 3, time_t invincibilityTime = 3):
            quantity(quantity), isInfinite(isInfinite),
            lives(lives), invTime(invincibilityTime) {};
    void setRasterizer(std::shared_ptr<Rasterizer> r) override;
    void Init() override;
    void Update(float dt) override;
    void Delete() override;
    bool DetectCollisions(Point p);
};
#endif //GAME_GAMEFIELDSCENE_H
