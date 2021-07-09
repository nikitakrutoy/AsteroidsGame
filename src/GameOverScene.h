//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_GAMEOVERSCENE_H
#define GAME_GAMEOVERSCENE_H

#include <cmath>
#include "Scene.h"
#include "MenuItem.h"
#include "GameFieldScene.h"

struct GameOverScene: Scene {
private:
    std::vector<MenuItem*> menuItems;
    size_t selectedMenuItem = 0;
    bool upWasPressed = false;
    bool downWasPressed = false;

public:
    GameOverScene() {typeIndex = typeid(*this);}
    void Init() override {
        position = Point(r->width / 2, r->height / 2);
        menuItems = std::vector<MenuItem*>({
            new SceneMenuItem<GameFieldScene>(
                    "Retry", position.Translate(Point(0, 100))),
            new ExitMenuItem(
                    "Exit", position.Translate(Point(0, 135)))
        });
        for (auto& m : menuItems) m->setRasterizer(r);
        menuItems[selectedMenuItem]->isSelected = true;
    };


protected:
    void Update(float dt) override{
        for (auto& m: menuItems) {m->SafeUpdate(dt);};
        if (is_key_pressed(VK_UP) && !upWasPressed) {
            upWasPressed = true;
        }

        if (!is_key_pressed(VK_UP) && upWasPressed) {
            menuItems[selectedMenuItem]->isSelected = false;
            selectedMenuItem = (menuItems.size() + selectedMenuItem - 1) % menuItems.size();
            menuItems[selectedMenuItem]->isSelected = true;
            upWasPressed = false;
        }

        if (is_key_pressed(VK_DOWN) && !downWasPressed) {
            downWasPressed = true;
        }

        if (!is_key_pressed(VK_DOWN) && downWasPressed) {
            menuItems[selectedMenuItem]->isSelected = false;
            selectedMenuItem = (menuItems.size() + selectedMenuItem + 1) % menuItems.size();
            menuItems[selectedMenuItem]->isSelected = true;
            downWasPressed = false;
        }
    }

    void Delete() override {
        for (auto& m : menuItems) {
            m->Delete();
            delete m;
        }
    }

protected:
    void Draw() override;
};


#endif //GAME_GAMEOVERSCENE_H
