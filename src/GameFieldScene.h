//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMEFIELDSCENE_H
#define GAME_GAMEFIELDSCENE_H

#include "Scene.h"
#include "LevelSpec.h"
#include "BackgroundScene.h"


struct GameFieldScene: Scene {
private:
    void InitAsteroids(size_t quantity);
    bool wasSpacePressed = false;
    bool isInfinite = false;
    const size_t quantity = 10;
    const size_t lives = 3;
    const time_t invTime = 3;
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
    void SetRasterizer(std::shared_ptr<Rasterizer> r) override;
    LevelSpec GetSpec() const {return LevelSpec(lives, invTime, quantity);}
    void Init() override;
    void Update(float dt) override;
    void Delete() override;
    bool DetectCollisions(Point p);
};
#endif //GAME_GAMEFIELDSCENE_H
