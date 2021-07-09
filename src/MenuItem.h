//
// Created by Nikita Krutoy on 09.07.2021.
//

#ifndef GAME_MENUITEM_H
#define GAME_MENUITEM_H

#include <cmath>
#include "GameObject.h"
#include "Engine.h"
#include "SceneManager.h"
#include "MagicNumbers.h"

struct MenuItem: GameObject {
    std::string text;
    bool isSelected = false;
    MenuItem(std::string text, Point p);
protected:
    void Draw() override{
        Color c = Color();
        if (isSelected) {
            float v = std::round((std::sin(elapsedTime * 10) + 1));
            c = c * v;
        }
        r->drawText(text, position, NORMAL_TEXT_SIZE, c, 2, true);
    }
    void Update(float dt) override{
        elapsedTime += dt;
    }
private:
    float elapsedTime = 0;
};

struct ExitMenuItem: MenuItem {
    ExitMenuItem(std::string text, Point p): MenuItem(text, p){};

protected:
    void Update(float dt) override {
        MenuItem::Update(dt);
        if (is_key_pressed(VK_RETURN) && isSelected) {
            schedule_quit_game();
        }
    }
};



struct SceneMenuItem: MenuItem  {
public:
    SceneMenuItem(std::string text, std::string sceneName, bool doReset, Point p):
        MenuItem(text, p), sceneName(sceneName), doReset(doReset){};

private:
    bool enterWasPressed = false;
    bool doReset = false;
    std::string sceneName;
protected:
    void Update(float dt) override{
        MenuItem::Update(dt);
        if (is_key_pressed(VK_RETURN) && !enterWasPressed) {
            enterWasPressed = true;
        }

        if (!is_key_pressed(VK_RETURN) && enterWasPressed && isSelected) {
            sceneManager.SetScene(sceneName);
            if (doReset) sceneManager.currentScene->Init();
            enterWasPressed = false;
        }
    }
};



#endif //GAME_MENUITEM_H
