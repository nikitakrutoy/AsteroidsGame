//
// Created by Nikita Krutoy on 08.07.2021.
//

#ifndef GAME_GAMEFIELDSCENE_CPP
#define GAME_GAMEFIELDSCENE_CPP

#include <random>
#include "Engine.h"
#include "SceneManager.h"
#include "GameState.h"
#include "GameFieldScene.h"
#include "GameOverScene.h"
#include "MagicNumbers.h"


void GameFieldScene::InitAsteroids(size_t quantity) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<float>  dist2(0, 1);
    std::uniform_int_distribution<float>  dist3(3, 5);
    asteroids.reserve(quantity);
    float radius;
    int power;
    for (int i = 0; i < quantity; i++) {
        power = dist3(generator);
        radius =  std::pow(2, power);
        asteroids.emplace_back(radius);
        asteroids[i].position = Point(
                dist2(generator) * r->width,
                dist2(generator) * r->height
        );
        asteroids[i].setRasterizer(r);
        asteroids[i].speed = 6 - power;

    }
};

void GameFieldScene::setRasterizer(std::shared_ptr<Rasterizer> r)  {
    this->r = r;

    player.setRasterizer(r);
    for (auto &a : asteroids) a.setRasterizer(r);
    for (auto &p : projectiles) p.setRasterizer(r);
}
void GameFieldScene::Init() {
    Path path = Path({Point(-25, -15), Point(30, 0), Point(-25, 15), Point(-10, 0)});
    player = SpaceObject(path);
    startPoint = Point(r->width / 2, r->height / 2);
    player.position = startPoint;
    player.setRasterizer(r);
    texts = std::vector<TextObject>(2);
    texts[0].position = Point(10, 10);
    texts[1].position = Point(10, 40);
    for (auto &t : texts) t.setRasterizer(r);
    GameState::lives = MAX_LIVES;
    GameState::score = 0;
    asteroids.clear();
    projectiles.clear();
    InitAsteroids(20);
};

void GameFieldScene::Draw()  {
    Color backgroundColor = Color(0.0f,0.0f,0.0f);
    r->fillColor(backgroundColor);
    player.SafeDraw();
    for (auto &a : asteroids) a.SafeDraw();
    for (auto &p : projectiles) p.SafeDraw();
    for (auto &t : texts) t.SafeDraw();
}

void GameFieldScene::Update(float dt) {
    if (GameState::lives <= 0) {
        sceneManager.SetScene<GameOverScene>();
        return;
    }
    player.SafeUpdate(dt);
    for (auto &a : asteroids) a.SafeUpdate(dt);
    for (auto &p : projectiles) p.SafeUpdate(dt);

    texts[0].text = "Score: " + std::to_string(GameState::score);
    texts[1].text = "Lives: " + std::to_string(GameState::lives);

    if (DetectCollisions(player.position)) {
        std::cout << "Starship crashed!" << std::endl;
        player.position = startPoint;
        player.velocity = Point(0, 0);
        GameState::lives -= 1;
    }

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        Point p = it->position;
        bool notInsideScreen = p.x > r->width || p.x < 0 || p.y > r->height || p.y < 0;
        if (notInsideScreen || DetectCollisions(p))
            projectiles.erase(it);
        else {
            ++it;
        }
    }


    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    if (is_key_pressed(VK_RIGHT))
        player.Rotate(2 * dt);

    if (is_key_pressed(VK_LEFT))
        player.Rotate(-2 * dt);

    if (is_key_pressed(VK_UP))
        player.Boost(0.1);

    if (is_key_pressed(VK_SPACE) && !wasSpacePressed) {
        wasSpacePressed = true;
    }

    if (!is_key_pressed(VK_SPACE) && wasSpacePressed) {
        Path empty = Path({});
        auto& a = typeid(asteroids);
        const std::type_info& ti1 = typeid(asteroids);
        const std::type_info& ti2 = typeid(asteroids);
        Projectile projectile = Projectile(empty);
        projectile.velocity = player.forwardDirection.Scale(5);
        projectile.position = player.position;
        projectile.setRasterizer(r);
        projectiles.push_back(projectile);
        wasSpacePressed = false;
    }
}


bool GameFieldScene::DetectCollisions(Point p) {
    bool result = false;
    std::vector<Asteroid>& as = asteroids;
    as.reserve(as.size() * 2);
    for (auto it = as.begin(); it != as.end();) {
        if (Distance(it->position, p) <= it->radius) {
            GameState::score += it->radius;
            if (it->radius / 2 >= 8) {
                Asteroid a1 = Asteroid(it->radius / 2);
                Asteroid a2 = Asteroid(it->radius / 2);
                a1.position = it->position.Translate(Point(it->radius / 2, 0));
                a2.position = it->position.Translate(Point(-it->radius / 2, 0));
                a1.speed = 6 - std::sqrt(it->radius);
                a2.speed = 6 - std::sqrt(it->radius);
                a1.setRasterizer(r); a2.setRasterizer(r);
                as.erase(it);
                as.push_back(a1);
                as.push_back(a2);
            } else {
                as.erase(it);
            }
            result = true;
        }
        else {
            ++it;
        }
    }
    as.shrink_to_fit();
    return result;
}

void GameFieldScene::Delete() {
    Scene::Delete();
    player.Delete();
    for (auto &a : asteroids) a.Delete();
    for (auto &p : projectiles) p.Delete();
    for (auto &t : texts) t.Delete();

}


#endif //GAME_GAMEFIELDSCENE_CPP
