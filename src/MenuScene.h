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
    virtual void Init() override {
        for (auto& m : menuItems) m->setRasterizer(r);
        for (auto& l : labels) l->setRasterizer(r);
        menuItems[selectedMenuItem]->isSelected = true;
    };


protected:
    size_t selectedMenuItem = 0;
    std::vector<MenuItem*> menuItems;
    std::vector<TextObject*> labels;
    void Update(float dt) override;

    void Delete() override {
        for (auto& m : menuItems) {
            m->Delete();
            delete m;
        }
    }

protected:
    void Draw() override;
};

struct GameOverScene: MenuScene {
    void Init() override;
};

struct PauseScene: MenuScene {
    void Init() override;
};

struct TitleScene: MenuScene {
    void Init() override;
};

struct LevelsScene: MenuScene {
    void Init() override;

protected:
    std::vector<std::string> descriptions;
    std::vector<LevelSpec> levelSpecs;
    void Update(float dt) override;
};


#endif //GAME_MENUSCENE_H
