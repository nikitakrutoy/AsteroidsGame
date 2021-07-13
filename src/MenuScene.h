//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_MENUSCENE_H
#define GAME_MENUSCENE_H

#include <cmath>
#include <utility>
#include "Scene.h"
#include "MenuItem.h"
#include "GameObject.h"
#include "GameFieldScene.h"

struct MenuScene: Scene {
private:

    bool upWasPressed = false;
    bool downWasPressed = false;

public:
    MenuScene() = default;
    virtual void Init() override;


protected:
    size_t selectedMenuItem = 0;
    std::vector<MenuItem*> menuItems;
    std::vector<TextObject*> labels;
    void Delete() override;
protected:
    void Update(float dt) override;
    void Draw() override;
};

struct EndGameScene: MenuScene {
    std::string title;
    void Init() override;
};

struct CompleteScene: EndGameScene {
    void Init() override {title = "Level Completed"; EndGameScene::Init();}
};

struct GameOverScene: EndGameScene {
    Billboard im;
    void Init() override;
protected:
    void Draw() override {
        EndGameScene::Draw();
        im.SafeDraw();
    }
};

struct PauseScene: MenuScene {
    void Init() override;
};

struct TitleScene: MenuScene {
    float elapsedTime = 0;
    void Init() override;

protected:
    void Update(float dt) override;
};

struct LevelsScene: MenuScene {
    void Init() override;

protected:
    std::vector<std::string> descriptions;
    std::vector<LevelSpec> levelSpecs;
    std::map<std::string, size_t> levelName2LabelIndex = {
            {"Level1", 5}, {"Level2", 6},
            {"Level3", 7}, {"Level4", 8}
    };
    void Update(float dt) override;
};


#endif //GAME_MENUSCENE_H
