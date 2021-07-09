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

    SpaceObject player{};
    std::vector<Asteroid> asteroids{};
    std::vector<Projectile> projectiles{};
    std::vector<TextObject> texts{};
    Point startPoint{};
protected:
    void Draw() override;

public:
    GameFieldScene() = default;
    void setRasterizer(std::shared_ptr<Rasterizer> r) override;
    void Init() override;
    void Update(float dt) override;
    bool DetectCollisions(Point p);
};
#endif //GAME_GAMEFIELDSCENE_H
