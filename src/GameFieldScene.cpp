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
#include "MagicNumbers.h"
#include "Geometry.h"

static std::random_device rand_dev;
static std::mt19937 generator(rand_dev());
static std::uniform_real_distribution<float>  dist2(0, 1);


void GameFieldScene::InitAsteroids(size_t quantity) {
    asteroids.reserve(quantity);
    float radius;
    int power;
    for (int i = 0; i < quantity; i++) {
        power = (MAX_ASTEROIDS_SIZE - 3) * dist2(generator) + 3;
        radius =  std::pow(2, power);
        asteroids.emplace_back(
                radius,
                Point(
                        dist2(generator) * r->width,
                        dist2(generator) * r->height
                ),
                MAX_ASTEROIDS_SIZE - power,
                !isInfinite
                );
        asteroids[i].SetRasterizer(r);
    }
};

void GameFieldScene::SetRasterizer(std::shared_ptr<Rasterizer> r)  {
    this->r = r;

    player.SetRasterizer(r);
    for (auto &a : asteroids) a.SetRasterizer(r);
    for (auto &p : projectiles) p.SetRasterizer(r);
}
void GameFieldScene::Init() {
    Path path = Path({
        Point(-25, -15), Point(30, 0), Point(-25, 15), Point(-10, 0)});
    player = Spaceship(path);
    startPoint = Point(r->width / 2, r->height / 2);
    player.position = startPoint;
    player.isSeamless = true;
    player.SetRasterizer(r);

    doUpdate = true;
    doDraw = true;
    enabled = true;


    scoreText = ScoreText("", Point(10, 10),
                          NORMAL_TEXT_SIZE, Color(),  2, false);
    livesText = LivesText("", Point(10, NORMAL_TEXT_SIZE + 20),
                          NORMAL_TEXT_SIZE, Color(),  2, false);

    scoreText.SetRasterizer(r);
    livesText.SetRasterizer(r);
    GameState::lives = lives;
    GameState::score = 0;
    asteroids.clear();
    projectiles.clear();
    InitAsteroids(quantity);
};

void GameFieldScene::Draw()  {
    player.SafeDraw();
    for (auto &a : asteroids) a.SafeDraw();
    for (auto &p : projectiles) p.SafeDraw();
    livesText.SafeDraw();
    scoreText.SafeDraw();
}

void GameFieldScene::Update(float dt) {
    if (GameState::lives <= 0) {
        sceneManager.SetBackgroundScene("SolidBackground");
        sceneManager.UnsetGameScene();
        sceneManager.SetUIScene("GameOver");
        return;
    }

    if(!isInfinite and asteroids.empty()) {
        sceneManager.SetBackgroundScene("SolidBackground");
        sceneManager.UnsetGameScene();
        sceneManager.SetUIScene("Complete");
        GameState::isLevelCompleted[sceneManager.lastGameSceneName] = true;
        GameState::Save();
        return;
    }

    player.SafeUpdate(dt);
    for (auto &a : asteroids) a.SafeUpdate(dt);
    for (auto &p : projectiles) p.SafeUpdate(dt);
    livesText.SafeUpdate(dt);
    scoreText.SafeUpdate(dt);


    if (invTimer == 0 && DetectCollisions(player.position)) {
        std::cout << "Starship crashed!" << std::endl;
        player.position = startPoint;
        player.velocity = Point(0, 0);
        player.enableInvincibility();
        invTimer = time(nullptr);
        GameState::lives -= 1;
    }

    if (invTimer > 0 && time(nullptr) - invTimer >= invTime) {
        invTimer = 0;
        player.disableInvincibility();
    }

    bool notInsideScreen;
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        Point p = it->position;
        notInsideScreen = p.x > r->width || p.x < 0 || p.y > r->height || p.y < 0;
        if (notInsideScreen || DetectCollisions(p))
            projectiles.erase(it);
        else ++it;

    }


    if (isInfinite) {
        int m = 100;
        for (auto &a: asteroids) {
            Point p = a.position;
            notInsideScreen = p.x > (r->width + m) || p.x < -m || (p.y > r->height + m) || (p.y < -m);
            if (notInsideScreen)
                a.position = Point(
                        frame2(p.x, -m, r->width + m),
                        frame2(p.y, -m, r->height + m)
                );
        }

        asteroids.reserve(20);
        float radius, power;
        while (asteroids.size() < 20) {
            power = (MAX_ASTEROIDS_SIZE - 3) * dist2(generator) + 3;
            radius =  std::pow(2, power);
            asteroids.emplace_back(radius,Point(-10, -10),
                                   1,!isInfinite);
            asteroids[asteroids.size() - 1].SetRasterizer(r);
            asteroids[asteroids.size() - 1].c = Color(1, 0, 0);
        }
        asteroids.shrink_to_fit();
    }

    if (is_key_pressed(VK_ESCAPE)){
        sceneManager.SetUIScene("Pause");
        doUpdate = false;
        return;
    }

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
        Projectile projectile = Projectile(empty);
        projectile.velocity = player.forwardDirection.Scale(5);
        projectile.position = player.position;
        projectile.SetRasterizer(r);
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
            GameState::score += 64 - it->radius / 2;
            if (it->radius / 2 >= 8) {
                Asteroid a1 = Asteroid(
                        it->radius / 2,
                        it->position.Translate(Point(it->radius / 2, 0)),
                        6 - std::log(it->radius),
                        !isInfinite);
                Asteroid a2 = Asteroid(
                        it->radius / 2,
                        it->position.Translate(Point(it->radius / 2, 0)),
                        6 - std::log(it->radius),
                        !isInfinite);
                a1.SetRasterizer(r);
                a2.SetRasterizer(r);
                as.erase(it);
                as.insert(as.end(), {a1, a2});
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
    livesText.Delete();
    scoreText.Delete();
}


#endif //GAME_GAMEFIELDSCENE_CPP
